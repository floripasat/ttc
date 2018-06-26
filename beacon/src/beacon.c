/*
 * beacon.c
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
 * 
 * This file is part of FloripaSat-Beacon.
 * 
 * FloripaSat-Beacon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-Beacon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-Beacon. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Beacon implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
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
#include "fsp/fsp.h"
#include "ngham/ngham.h"
#include "ax25/ax25.h"

Beacon beacon;

void beacon_init()
{
#if BEACON_MODE == DEBUG_MODE
    watchdog_hold();    // Disable watchdog for debug
    task_init_with_timeout(&debug_init, DEBUG_INIT_TIMEOUT_MS);
#else
    watchdog_init();
#endif // DEBUG_MODE
    
    cpu_init();

    flash_init();
    
#if BEACON_MODE != FLIGHT_MODE
    status_led_init();
#endif // FLIGHT_MODE
    
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
    
    beacon.hibernation                  = false;
    beacon.can_transmit                 = true;
    beacon.transmitting                 = false;
    beacon.energy_level                 = SATELLITE_ENERGY_LEVEL_5;
    beacon.last_radio_reset_time        = time_get_seconds();
    beacon.last_system_reset_time       = time_get_seconds();
    beacon.last_ngham_pkt_transmission  = time_get_seconds();
    beacon.last_devices_verification    = time_get_seconds();
    beacon.last_energy_level_set        = time_get_seconds();
    
    beacon.eps.time_last_valid_pkt      = time_get_seconds();
    beacon.eps.errors                   = 0;
    beacon.eps.is_dead                  = false;

    beacon.obdh.time_last_valid_pkt     = time_get_seconds();
    beacon.obdh.errors                  = 0;
    beacon.obdh.is_dead                 = false;
}

void beacon_deinit()
{
    ngham_deinit_arrays();
}

void beacon_run()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Running...\n");
#elif BEACON_MODE == FLIGHT_MODE
    if (!antenna_is_released())
    {
        task_antenna_deployment();
    }
#endif // BEACON_MODE
    
    __enable_interrupt();
    
    while(1)
    {
        task_periodic(&beacon_check_devices_status, 1, &beacon.last_devices_verification, time_get_seconds());
        
    #if BEACON_PACKET_PROTOCOL & PACKET_NGHAM
        task_periodic_no_preemption(&beacon_send_ngham_pkt, beacon_get_tx_period(), &beacon.last_ngham_pkt_transmission, time_get_seconds());
    #endif // PACKET_NGHAM
        
    #if BEACON_PACKET_PROTOCOL & PACKET_AX25
        task_scheduled_no_preemption(&beacon_send_ax25_pkt, beacon.last_ngham_pkt_transmission + 1, time_get_seconds(), 0, ((beacon.last_ngham_pkt_transmission + 1) == time_get_seconds())? true : false);
    #endif // PACKET_AX25
        
        task_aperiodic(&beacon_process_obdh_pkt, obdh_available()? true : false);
        
        task_aperiodic(&beacon_process_eps_pkt, eps_available()? true : false);
        
        task_aperiodic(&beacon_process_radio_pkt, (radio_available()? true : false) && (beacon.obdh.is_dead? true : false));
        
        task_aperiodic(&radio_enable_rx, beacon.obdh.is_dead? true : false);
        
        task_scheduled(&beacon_leave_hibernation, beacon.hibernation_mode_initial_time + BEACON_HIBERNATION_PERIOD_SECONDS, time_get_seconds(), 5, beacon.hibernation? true : false);
        
        task_periodic(&beacon_set_energy_level, BEACON_TX_PERIOD_SEC_L1, &beacon.last_energy_level_set, time_get_seconds());
        
        task_periodic(&radio_init, BEACON_RADIO_RESET_PERIOD_SEC, &beacon.last_radio_reset_time, time_get_seconds());
        
        task_periodic(&system_reset, BEACON_SYSTEM_RESET_PERIOD_SEC, &beacon.last_system_reset_time, time_get_seconds());
        
    #if BEACON_MODE != FLIGHT_MODE
        status_led_toggle();                // Heartbeat
    #endif // BEACON_MODE
        
        system_enter_low_power_mode();      // Wait until the time timer execution (When the system leaves low-power mode)
        
    #if BEACON_MODE != DEBUG_MODE
        watchdog_reset_timer();
    #endif // BEACON_MODE
    }
}

void beacon_enter_hibernation()
{
    if (!beacon.hibernation)
    {
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("Entering in hibernation mode... ");
    #endif // DEBUG_MODE
        
        radio_sleep();
        
        beacon.hibernation = true;
        
        beacon.hibernation_mode_initial_time = time_get_seconds();
        
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("DONE!\n");
    #endif // DEBUG_MODE
    }
}

void beacon_leave_hibernation()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Leaving hibernation mode... ");
#endif // DEBUG_MODE
    
    radio_wake_up();
    
    beacon.hibernation = false;
    
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
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
}

void beacon_check_devices_status()
{
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
}

void beacon_gen_pkt_payload()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Generating packet payload from ");
#endif // DEBUG_MODE
    
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
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("OBDH data... ");
    #endif // DEBUG_MODE
        
        buffer_append(&beacon.pkt_payload, beacon.obdh.buffer.data, beacon.obdh.buffer.size);
        
#endif // PAYLOAD_OBDH_DATA
    }
    else if ((beacon.eps.errors == 0) && (!beacon.eps.is_dead))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_EPS_DATA
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("EPS data... ");
    #endif // DEBUG_MODE
        
        buffer_append(&beacon.pkt_payload, beacon.eps.buffer.data, beacon.eps.buffer.size);
        
#endif // PAYLOAD_EPS_DATA
    }
    else
    {
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("the satellite ID... ");
#endif // DEBUG_MODE
    }
    
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
}

void beacon_gen_ngham_pkt(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len)
{
    beacon_gen_pkt_payload();
    
    NGHam_TX_Packet ngham_packet;
    
    ngham_tx_pkt_gen(&ngham_packet, beacon.pkt_payload.data, beacon.pkt_payload.size);
    ngham_encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);
}

void beacon_gen_ax25_pkt(uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len)
{
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
            beacon.transmitting = true;
            
            uint8_t ax25_pkt_str[256];
            uint16_t ax25_pkt_str_len;
            
            beacon_gen_ax25_pkt(ax25_pkt_str, &ax25_pkt_str_len);
            
            radio_write(ax25_pkt_str, ax25_pkt_str_len);
            
            beacon.transmitting = false;
        }
    }
}

void beacon_process_obdh_pkt()
{
    FSPPacket obdh_pkt;
    
    uint8_t fsp_state;
    
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
        
        while(radio_available())
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
                
            #if BEACON_MODE == DEBUG_MODE
                debug_print_msg("Shutdown command received from ");
                for(i=0; i<6; i++)
                {
                    debug_print_byte(data[i]);
                }
                debug_print_msg("!\n");
            #endif // DEBUG_MODE
                
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
    uint8_t time_marker = time_get_seconds();
    
    while((time_get_seconds() - time_marker) <= BEACON_ANTENNA_DEPLOY_SLEEP_SEC)
    {
#if BEACON_MODE != DEBUG_MODE
        watchdog_reset_timer();
#endif // BEACON_MODE
        
        system_enter_low_power_mode();
    }
    
    antenna_deploy();
}

void beacon_delay_sec(uint8_t delay_sec)
{
    uint8_t i = 0;
    for(i=0; i<delay_sec; i++)
    {
        system_enter_low_power_mode();

#if BEACON_MODE != DEBUG_MODE
        watchdog_reset_timer();
#endif // BEACON_MODE
    }
}

//! \} End of beacon group
