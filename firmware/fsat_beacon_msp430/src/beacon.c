/*
 * beacon.c
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-TTC.
 * 
 * FloripaSat-TTC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-TTC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Beacon implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.1
 * 
 * \date 08/06/2017
 * 
 * \addtogroup beacon
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <hal/hal.h>
#include <system/system.h>

#include "beacon.h"
#include "beacon_config.h"
#include "fsp/fsp.h"
#include "ngham/ngham.h"
#include "ax25/ax25.h"

Beacon beacon;

void beacon_init()
{
    watchdog_init();
    
    cpu_init();

    debug_init();

    status_led_init();
    
    time_init();

    time_timer_start();

    __enable_interrupt();
    beacon_delay_sec(BEACON_BOOT_DELAY_SEC);
    __disable_interrupt();

    task_init_with_timeout(&antenna_init, BEACON_ANTENNA_INIT_TIMEOUT_MS);
    
    task_init_with_timeout(&eps_init, EPS_INIT_TIMEOUT_MS);
    
    task_init_with_timeout(&obdh_init, OBDH_INIT_TIMEOUT_MS);
    
    task_init_with_timeout(&radio_init, RADIO_INIT_TIMEOUT_MS);
    
#if BEACON_RF_SWITCH != HW_NONE
    rf_switch_init();
#endif // BEACON_RF_SWITCH
    
#if BEACON_PA != HW_NONE
    task_init_with_timeout(&pa_init, BEACON_PA_INIT_TIMEOUT_MS);
#endif // BEACON_PA
    
    fsp_init(FSP_ADR_TTC);
    
    ngham_init();

#if BEACON_RESET_PARAMS_ON_BOOT == 1
    beacon_reset_params();
#else
    beacon_load_params();
#endif // BEACON_RESET_PARAMS

    beacon.can_transmit                 = true;
    beacon.transmitting                 = false;
    beacon.last_radio_reset_time        = time_get_seconds();
    beacon.last_system_reset_time       = time_get_seconds();
    beacon.last_ngham_pkt_transmission  = time_get_seconds();
    beacon.last_devices_verification    = time_get_seconds();
}

void beacon_deinit()
{
    ngham_deinit_arrays();
}

void beacon_run()
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Running main loop...\n\r");

    __enable_interrupt();

    while(1)
    {
        task_aperiodic(&beacon_antenna_deployment, beacon.deployment_executed? false : true);

        task_periodic(&beacon_check_devices_status, 1, &beacon.last_devices_verification, time_get_seconds());

    #if BEACON_PACKET_PROTOCOL & PACKET_NGHAM
        task_periodic_no_preemption(&beacon_send_ngham_pkt, beacon_get_tx_period(), &beacon.last_ngham_pkt_transmission, time_get_seconds());
    #endif // PACKET_NGHAM

    #if BEACON_PACKET_PROTOCOL & PACKET_AX25
        task_scheduled_no_preemption(&beacon_send_ax25_pkt, beacon.last_ngham_pkt_transmission + 1, time_get_seconds(), 0, ((beacon.last_ngham_pkt_transmission + 1) == time_get_seconds())? true : false);
    #endif // PACKET_AX25

        task_aperiodic(&beacon_process_obdh_pkt, obdh_available()? true : false);

        task_aperiodic(&beacon_process_eps_pkt, eps_available()? true : false);

        task_aperiodic(&beacon_process_radio_pkt, ((radio_available() > 0)? true : false) && (beacon.obdh.is_dead? true : false));

        task_aperiodic(&radio_enable_rx, beacon.obdh.is_dead? true : false);

        task_scheduled(&beacon_leave_hibernation, beacon.hibernation_mode_initial_time + BEACON_HIBERNATION_PERIOD_SECONDS, time_get_seconds(), 5, beacon.hibernation? true : false);

        task_periodic(&beacon_set_energy_level, BEACON_TX_PERIOD_SEC_L1, &beacon.last_energy_level_set, time_get_seconds());

        task_periodic(&radio_init, BEACON_RADIO_RESET_PERIOD_SEC, &beacon.last_radio_reset_time, time_get_seconds());

        task_periodic(&system_reset, BEACON_SYSTEM_RESET_PERIOD_SEC, &beacon.last_system_reset_time, time_get_seconds());

        task_periodic(&beacon_save_params, BEACON_SAVE_PARAMS_PERIOD_S, &beacon.last_params_saving, time_get_seconds());

        status_led_toggle();                // Heartbeat

        system_enter_low_power_mode();      // Wait until the time timer execution (When the system leaves low-power mode)

        watchdog_reset_timer();
    }
}

void beacon_enter_hibernation()
{
    if (!beacon.hibernation)
    {
        debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Entering in hibernation mode...\n\r");

        radio_sleep();

        beacon.hibernation = true;

        beacon.hibernation_mode_initial_time = time_get_seconds();
    }
}

void beacon_leave_hibernation()
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Leaving hibernation mode...\n\r");

    radio_wake_up();

    beacon.hibernation = false;
}

uint8_t beacon_get_tx_period()
{
    switch(beacon.energy_level)
    {
        case SATELLITE_ENERGY_LEVEL_1:
            return BEACON_TX_PERIOD_SEC_L1;
        case SATELLITE_ENERGY_LEVEL_2:
            return BEACON_TX_PERIOD_SEC_L2;
        case SATELLITE_ENERGY_LEVEL_3:
            return BEACON_TX_PERIOD_SEC_L3;
        case SATELLITE_ENERGY_LEVEL_4:
            return BEACON_TX_PERIOD_SEC_L4;
        case SATELLITE_ENERGY_LEVEL_5:
            return BEACON_TX_PERIOD_SEC_L5;
        default:
            return BEACON_TX_PERIOD_SEC_L5;
    }
}

void beacon_set_energy_level()
{
    uint8_t last_energy_level = beacon.energy_level;

    if ((beacon.obdh.errors == 0) && (!beacon.obdh.is_dead))
    {
        beacon.energy_level = beacon.obdh.buffer.data[OBDH_PKT_ENERGY_LEVEL_POS];
    }
    else if ((beacon.eps.errors == 0) && (!beacon.eps.is_dead))
    {
        beacon.energy_level = beacon.eps.buffer.data[EPS_PKT_ENERGY_LEVEL_POS];
    }
    else
    {
        beacon.energy_level = SATELLITE_ENERGY_LEVEL_5;
    }

    if (last_energy_level != beacon.energy_level)
    {
        debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Changing energy level from ");
        debug_print_dec(last_energy_level);
        debug_print_msg(" to ");
        debug_print_dec(beacon.energy_level);
        debug_print_msg("!\n\r");
    }
}

void beacon_check_devices_status()
{
    bool last_obdh_status = beacon.obdh.is_dead;
    bool last_eps_status = beacon.eps.is_dead;

    if ((time_get_seconds() - beacon.obdh.time_last_valid_pkt) <= OBDH_TIMEOUT_SEC)
    {
        beacon.obdh.is_dead = false;
    }
    else
    {
        beacon.obdh.is_dead = true;
    }
    
    if ((time_get_seconds() - beacon.eps.time_last_valid_pkt) <= EPS_TIMEOUT_SEC)
    {
        beacon.eps.is_dead = false;
    }
    else
    {
        beacon.eps.is_dead = true;
    }
    
    if (!beacon.can_transmit)
    {
        if ((time_get_seconds() - beacon.time_obdh_started_tx) >= OBDH_RADIO_TIMEOUT_SEC)
        {
            beacon.can_transmit = true;
        }
    }
    
    // Antenna connection status
    // Radio status

    if (last_obdh_status != beacon.obdh.is_dead)
    {
        debug_print_event_from_module(DEBUG_ERROR, BEACON_MODULE_NAME, "The OBDH module is not responding!\n\r");
    }

    if (last_eps_status != beacon.eps.is_dead)
    {
        debug_print_event_from_module(DEBUG_ERROR, BEACON_MODULE_NAME, "The EPS module is not responding!\n\r");
    }
}

void beacon_gen_pkt_payload()
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Generating packet payload from ");

    if (!buffer_empty(&beacon.pkt_payload))
    {
        buffer_clear(&beacon.pkt_payload);
    }

#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_SAT_ID
    buffer_fill(&beacon.pkt_payload, SATELLITE_ID, sizeof(SATELLITE_ID)-1);
#endif // PAYLOAD_SAT_ID
    
    if ((beacon.obdh.errors == 0) && (!beacon.obdh.is_dead))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_OBDH_DATA
        debug_print_msg("OBDH data...\n\r");

        buffer_append(&beacon.pkt_payload, beacon.obdh.buffer.data, beacon.obdh.buffer.size);
        
#endif // PAYLOAD_OBDH_DATA
    }
    else if ((beacon.eps.errors == 0) && (!beacon.eps.is_dead))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_EPS_DATA
        debug_print_msg("EPS data...\n\r");

        buffer_append(&beacon.pkt_payload, beacon.eps.buffer.data, beacon.eps.buffer.size);
        
#endif // PAYLOAD_EPS_DATA
    }
    else
    {
        debug_print_msg("the satellite ID...\n\r");
    }
}

void beacon_gen_ngham_pkt(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len)
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Generating a NGHam packet...\n\r");

    beacon_gen_pkt_payload();

    NGHam_TX_Packet ngham_packet;

    ngham_tx_pkt_gen(&ngham_packet, beacon.pkt_payload.data, beacon.pkt_payload.size);
    ngham_encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);
}

void beacon_gen_ax25_pkt(uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len)
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Generating a AX.25 packet...\n\r");

    beacon_gen_pkt_payload();

    AX25_Packet ax25_packet;

    ax25_beacon_pkt_gen(&ax25_packet, beacon.pkt_payload.data, beacon.pkt_payload.size);
    ax25_encode(&ax25_packet, ax25_pkt_str, ax25_pkt_str_len);
}

void beacon_send_ngham_pkt()
{
    if (!beacon.hibernation)
    {
        if (beacon.can_transmit)
        {
            debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Transmitting a NGHam packet...\n\r");

            uint8_t ngham_pkt_str[256];
            uint16_t ngham_pkt_str_len;

            beacon_gen_ngham_pkt(ngham_pkt_str, &ngham_pkt_str_len);

            beacon.transmitting = true;

            radio_write(ngham_pkt_str+8, ngham_pkt_str_len-8);  // 8: Removing preamble and sync word from the NGHam packet

            beacon.transmitting = false;
        }
    }
}

void beacon_send_ax25_pkt()
{
    if (!beacon.hibernation)
    {
        if (beacon.can_transmit)
        {
            debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Transmitting a AX.25 packet...\n\r");

            uint8_t ax25_pkt_str[256];
            uint16_t ax25_pkt_str_len;

            beacon_gen_ax25_pkt(ax25_pkt_str, &ax25_pkt_str_len);

            beacon.transmitting = true;

            radio_write(ax25_pkt_str, ax25_pkt_str_len);

            beacon.transmitting = false;
        }
    }
}

void beacon_process_obdh_pkt()
{
    FSPPacket obdh_pkt;
    
    uint8_t fsp_state;
    
    fsp_reset();

    while(obdh_available())
    {
        fsp_state = fsp_decode(obdh_pop(), &obdh_pkt);
        
        if (fsp_state == FSP_PKT_READY)
        {
            break;
        }
        else if (fsp_state == FSP_PKT_INVALID)
        {
            beacon.obdh.errors++;
            
            return;
        }
        else
        {
            continue;
        }
    }
    
    if (fsp_state == FSP_PKT_READY)     // Only process a full received packet
    {
        // Checking if the packet is really from the OBDH module
        if (obdh_pkt.src_adr != FSP_ADR_OBDH)
        {
            beacon.obdh.errors++;
            
            return;
        }
        
        switch(obdh_pkt.type)
        {
            case FSP_PKT_TYPE_DATA:
                buffer_fill(&beacon.obdh.buffer, obdh_pkt.payload, obdh_pkt.length);
                
                beacon.obdh.time_last_valid_pkt = time_get_seconds();
                beacon.obdh.errors = 0;
                
                return;
            case FSP_PKT_TYPE_DATA_WITH_ACK:
                buffer_fill(&beacon.obdh.buffer, obdh_pkt.payload, obdh_pkt.length);
                
                beacon.obdh.time_last_valid_pkt = time_get_seconds();
                beacon.obdh.errors = 0;
                
                FSPPacket obdh_ack_pkt;
                
                fsp_gen_ack_pkt(FSP_ADR_OBDH, &obdh_ack_pkt);
                
                uint8_t pkt[FSP_PKT_MAX_LENGTH];
                uint8_t pkt_len;
                
                fsp_encode(&obdh_ack_pkt, pkt, &pkt_len);
                
                obdh_send(pkt, pkt_len);
                
                return;
        }
        
        if (obdh_pkt.type == FSP_PKT_TYPE_CMD)
        {
            switch(obdh_pkt.payload[0])
            {
                case FSP_CMD_NOP:                   // Nothing to do.
                    break;
                case FSP_CMD_SEND_DATA:             // The Beacon module does not send data to others module.
                    break;
                case FSP_CMD_REQUEST_RF_MUTEX:      // This command demands an cmd. with ack.
                    break;
                case FSP_CMD_SHUTDOWN:
                    beacon_enter_hibernation();
                    break;
            }
        }
        
        if (obdh_pkt.type == FSP_PKT_TYPE_CMD_WITH_ACK)
        {
            FSPPacket obdh_ack_pkt;
            
            switch(obdh_pkt.payload[0])
            {
                case FSP_CMD_NOP:
                    fsp_gen_ack_pkt(FSP_ADR_OBDH, &obdh_ack_pkt);
                    break;
                case FSP_CMD_SEND_DATA:
                    fsp_gen_nack_pkt(FSP_ADR_OBDH, &obdh_ack_pkt);
                    break;
                case FSP_CMD_REQUEST_RF_MUTEX:
                    if (beacon.transmitting == true)
                    {
                        fsp_gen_pkt((uint8_t*)FSP_ACK_RF_MUTEX_BUSY, 1, FSP_ADR_OBDH, FSP_PKT_TYPE_ACK, &obdh_ack_pkt);
                    }
                    else
                    {
                        fsp_gen_pkt((uint8_t*)FSP_ACK_RF_MUTEX_FREE, 1, FSP_ADR_OBDH, FSP_PKT_TYPE_ACK, &obdh_ack_pkt);
                    }
                    break;
                case FSP_CMD_SHUTDOWN:
                    beacon_enter_hibernation();
                    fsp_gen_ack_pkt(FSP_ADR_OBDH, &obdh_ack_pkt);
                    break;
            }
            
            uint8_t pkt[FSP_PKT_MAX_LENGTH];
            uint8_t pkt_len;
            
            fsp_encode(&obdh_ack_pkt, pkt, &pkt_len);
            
            beacon.time_obdh_started_tx = time_get_seconds();
            beacon.can_transmit = false;
            
            obdh_send(pkt, pkt_len);
        }
    }
}

void beacon_process_eps_pkt()
{
    FSPPacket eps_pkt;
    
    uint8_t fsp_state;
    
    fsp_reset();

    while(eps_available())
    {
        fsp_state = fsp_decode(eps_pop(), &eps_pkt);
        
        if (fsp_state == FSP_PKT_READY)
        {
            break;
        }
        else if (fsp_state == FSP_PKT_INVALID)
        {
            beacon.eps.errors++;
            
            return;
        }
        else
        {
            continue;
        }
    }
    
    if (fsp_state == FSP_PKT_READY)     // Only process a full received packet
    {
        // Checking if the packet is really from the EPS module
        if (eps_pkt.src_adr != FSP_ADR_EPS)
        {
            beacon.eps.errors++;
            
            return;
        }
        
        switch(eps_pkt.type)
        {
            case FSP_PKT_TYPE_DATA:
                buffer_fill(&beacon.eps.buffer, eps_pkt.payload, eps_pkt.length);
                
                beacon.eps.time_last_valid_pkt = time_get_seconds();
                beacon.eps.errors = 0;
                
                return;
            default:
                beacon.eps.errors++;
                
                return;
        }
    }
}

void beacon_process_radio_pkt()
{
    if (!beacon.hibernation)
    {
        uint8_t data[100];
        uint8_t data_len;

        uint8_t ngham_state;

        while(radio_available() > 0)
        {
            ngham_state = ngham_decode(radio_pop(), data, &data_len);

            if (ngham_state == PKT_CONDITION_OK)
            {
                break;
            }
            else if (ngham_state == PKT_CONDITION_FAIL)
            {
                return;
            }
            else
            {
                continue;
            }
        }

        if (ngham_state == PKT_CONDITION_OK)
        {        
            if ((data[6] == 's') && (data[7] == 'd'))
            {
                uint8_t i = 0;

                debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Shutdown command received from ");
                for(i=0; i<6; i++)
                {
                    debug_print_byte(data[i]);
                }
                debug_print_msg("!\n\r");

                uint8_t ngham_pkt_str[100];
                uint16_t ngham_pkt_str_len;

                uint8_t pkt_payload_len = 0;
                uint8_t pkt_payload[60];

                uint8_t shutdown_ack_start[] = "Shutdown received from ";

                for(i=0; i<sizeof(shutdown_ack_start); i++)
                {
                    pkt_payload[pkt_payload_len++] = shutdown_ack_start[i];
                }

                for(i=0; i<6; i++)
                {
                    pkt_payload[pkt_payload_len++] = data[i];
                }

                uint8_t shutdown_ack_end[] = ". Wake up time in 24 hours.";
                for(i=0; i<sizeof(shutdown_ack_end); i++)
                {
                    pkt_payload[pkt_payload_len++] = shutdown_ack_end[i];
                }

                NGHam_TX_Packet ngham_packet;
                ngham_tx_pkt_gen(&ngham_packet, pkt_payload, pkt_payload_len);
                ngham_encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);

                uint32_t timeout_radio_shutdown_ack = BEACON_TIMEOUT_RADIO_SHUTDOWN;

                while(timeout_radio_shutdown_ack--)
                {
                    if (beacon.can_transmit)
                    {
                        beacon.transmitting = true;

                        radio_write(ngham_pkt_str, ngham_pkt_str_len);

                        beacon.transmitting = false;

                        break;
                    }
                }

                beacon_enter_hibernation();
            }
        }
    }
}

void beacon_antenna_deployment()
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Executing the deployment routines...\n\r");

    if (beacon.deployment_attempts >= BEACON_ANTENNA_MAX_DEPLOYMENTS)
    {
        debug_print_event_from_module(DEBUG_WARNING, BEACON_MODULE_NAME, "Enough deployment attempts executed (");
        debug_print_dec(beacon.deployment_attempts);
        debug_print_msg(")! Skipping the deployment routine...\n\r");

        beacon.hibernation = false;
        beacon.deployment_executed = true;

        return;
    }

    // If it is the first deployment attempt, wait 45 minutes before trying to deploy
    if (!beacon.deploy_hibernation_executed)
    {
        beacon.hibernation = true;

        debug_print_event_from_module(DEBUG_WARNING, BEACON_MODULE_NAME, "Deployment never executed! First deployment attempt in ");
        debug_print_dec(BEACON_ANTENNA_DEPLOY_SLEEP_MIN);
        debug_print_msg(" minute(s)...\n\r");

        uint32_t time_marker = time_get_seconds();
        uint32_t time_marker_minutes = time_get_minutes();

        while((time_get_seconds() - time_marker) <= BEACON_ANTENNA_DEPLOY_SLEEP_SEC)
        {
            watchdog_reset_timer();

            system_enter_low_power_mode();

            if (((time_get_seconds() - time_marker) % 60) == 0)
            {
                debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "First deployment attempt in ");
                debug_print_dec((BEACON_ANTENNA_DEPLOY_SLEEP_MIN + time_marker_minutes) - time_get_minutes());
                debug_print_msg(" minute(s)...\n\r");
            }
        }

        beacon.deploy_hibernation_executed = true;

        beacon_save_params();
    }

    antenna_deploy();

    beacon.deployment_attempts++;

    beacon.hibernation = false;
    beacon.deployment_executed = true;
}

void beacon_delay_sec(uint8_t delay_sec)
{
    uint8_t i = 0;
    for(i=0; i<delay_sec; i++)
    {
        system_enter_low_power_mode();

        watchdog_reset_timer();
    }
}

void beacon_load_params()
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Loading the system parameters from the flash memory...\n\r");

    beacon.params_saved = flash_read_single(BEACON_PARAM_PARAMS_SAVED_MEM_ADR) == 1 ? true : false;

    if (!beacon.params_saved)
    {
        debug_print_event_from_module(DEBUG_WARNING, BEACON_MODULE_NAME, "No saved system parameters found! Loading default values...\n\r");

        beacon_load_default_params();
    }
    else
    {
        beacon.hibernation                  = (bool)flash_read_single(BEACON_PARAM_HIBERNATION_MEM_ADR);
        beacon.energy_level                 = flash_read_single(BEACON_PARAM_ENERGY_LEVEL_MEM_ADR);
        beacon.last_energy_level_set        = flash_read_long(BEACON_PARAM_LAST_ENERGY_LEVEL_SET_MEM_ADR);
        beacon.deploy_hibernation_executed  = flash_read_single(BEACON_PARAM_PARAMS_DEPLOU_HIB_EXECUTED_MEM_ADR);
        beacon.deployment_attempts          = flash_read_single(BEACON_PARAM_DEPLOYMENT_ATTEMPTS_MEM_ADR);

        beacon.eps.time_last_valid_pkt      = flash_read_long(BEACON_PARAM_EPS_LAST_TIME_VALID_PKT_MEM_ADR);
        beacon.eps.errors                   = flash_read_single(BEACON_PARAM_EPS_ERRORS_MEM_ADR);
        beacon.eps.is_dead                  = (bool)flash_read_single(BEACON_PARAM_EPS_IS_DEAD_PKT_MEM_ADR);

        beacon.obdh.time_last_valid_pkt     = flash_read_long(BEACON_PARAM_OBDH_LAST_TIME_VALID_PKT_MEM_ADR);
        beacon.obdh.errors                  = flash_read_single(BEACON_PARAM_EPS_ERRORS_MEM_ADR);
        beacon.obdh.is_dead                 = (bool)flash_read_single(BEACON_PARAM_OBDH_IS_DEAD_PKT_MEM_ADR);
    }
}

void beacon_load_default_params()
{
    beacon.hibernation                  = false;
    beacon.energy_level                 = SATELLITE_ENERGY_LEVEL_5;
    beacon.deploy_hibernation_executed  = false;
    beacon.last_energy_level_set        = time_get_seconds();
    beacon.deployment_attempts          = 0;

    beacon.eps.time_last_valid_pkt      = time_get_seconds();
    beacon.eps.errors                   = 0;
    beacon.eps.is_dead                  = false;

    beacon.obdh.time_last_valid_pkt     = time_get_seconds();
    beacon.obdh.errors                  = 0;
    beacon.obdh.is_dead                 = false;
}

void beacon_save_params()
{
    debug_print_event_from_module(DEBUG_INFO, BEACON_MODULE_NAME, "Saving the system parameters to the flash memory...\n\r");

    flash_erase(BEACON_PARAMS_MEMORY_REGION);

    flash_write_single(beacon.hibernation ? 1 : 0, BEACON_PARAM_HIBERNATION_MEM_ADR);
    flash_write_single(beacon.energy_level, BEACON_PARAM_ENERGY_LEVEL_MEM_ADR);
    flash_write_long(beacon.last_energy_level_set, BEACON_PARAM_LAST_ENERGY_LEVEL_SET_MEM_ADR);
    flash_write_single(beacon.deploy_hibernation_executed ? 1 : 0, BEACON_PARAM_PARAMS_DEPLOU_HIB_EXECUTED_MEM_ADR);
    flash_write_single(beacon.deployment_attempts, BEACON_PARAM_DEPLOYMENT_ATTEMPTS_MEM_ADR);

    flash_write_long(beacon.eps.time_last_valid_pkt, BEACON_PARAM_EPS_LAST_TIME_VALID_PKT_MEM_ADR);
    flash_write_single(beacon.eps.errors, BEACON_PARAM_EPS_ERRORS_MEM_ADR);
    flash_write_single(beacon.eps.is_dead ? 1 : 0, BEACON_PARAM_EPS_IS_DEAD_PKT_MEM_ADR);

    flash_write_long(beacon.obdh.time_last_valid_pkt, BEACON_PARAM_OBDH_LAST_TIME_VALID_PKT_MEM_ADR);
    flash_write_single(beacon.obdh.errors, BEACON_PARAM_EPS_ERRORS_MEM_ADR);
    flash_write_single(beacon.obdh.is_dead ? 1 : 0, BEACON_PARAM_OBDH_IS_DEAD_PKT_MEM_ADR);

    flash_write_single(1, BEACON_PARAM_PARAMS_SAVED_MEM_ADR);

    beacon.last_params_saving = time_get_seconds();
}

void beacon_reset_params()
{
    debug_print_event_from_module(DEBUG_WARNING, BEACON_MODULE_NAME, "Reseting the system parameters...\n\r");

    beacon_load_default_params();

    beacon_save_params();
}

//! \} End of beacon group
