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
 * \file beacon.c
 * 
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

#include <msp430.h>
#include <stdbool.h>
#include <config/config.h>
#include <modules/modules.h>
#include <libs/libs.h>

#include "beacon.h"
#include "init.h"
#include "tasks.h"

Beacon beacon;

void beacon_init()
{
#if BEACON_MODE == DEBUG_MODE
    init_debug();
#else
    init_watchdog();
#endif // DEBUG_MODE

    init_cpu();

    init_memory();

#if BEACON_MODE != FLIGHT_MODE
    init_status_led();
#endif // FLIGHT_MODE

    init_time(&beacon.second_counter);
    
    init_antenna();
    
    init_eps_com(&beacon.eps, &beacon.second_counter);
    
    init_obdh_com(&beacon.obdh, &beacon.second_counter);
    
    init_radio();
    
#if BEACON_RF_SWITCH != HW_NONE
    init_rf_switch();
#endif // BEACON_RF_SWITCH

#if BEACON_PA != HW_NONE    
    init_pa();
#endif // BEACON_PA
    
    init_protocols();
    
    beacon.last_radio_reset_time        = beacon.second_counter;
    beacon.last_system_reset_time       = beacon.second_counter;
    beacon.last_ngham_pkt_transmission  = beacon.second_counter;
    beacon.flags.hibernation            = false;
    beacon.flags.can_transmit           = true;
    beacon.flags.transmitting           = false;
    beacon.energy_level                 = SATELLITE_ENERGY_LEVEL_5;
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
        task_periodic(&beacon_check_devices_status, BEACON_TX_PERIOD_SEC_L1, &beacon.last_devices_verification, beacon.second_counter);
        
    #if BEACON_PACKET_PROTOCOL & PACKET_NGHAM
        task_periodic_no_preemption(&beacon_transmit_ngham_packet, beacon_get_tx_period(), &beacon.last_ngham_pkt_transmission, beacon.second_counter);
    #endif // PACKET_NGHAM
        
    #if BEACON_PACKET_PROTOCOL & PACKET_AX25
        task_scheduled_no_preemption(&beacon_transmit_ax25_packet, beacon.last_ngham_pkt_transmission + 1, beacon.second_counter, 0, true);
    #endif // PACKET_AX25
        
        task_aperiodic(&radio_enable_rx, beacon.obdh.is_dead? true : false);
        
        task_aperiodic(&beacon_enter_hibernation, beacon.obdh.enter_hibernation? true : false);
        
        task_scheduled(&beacon_leave_hibernation, beacon.hibernation_mode_initial_time + BEACON_HIBERNATION_PERIOD_SECONDS, beacon.second_counter, 5, beacon.flags.hibernation? true : false);
        
        task_periodic(&beacon_set_energy_level, BEACON_TX_PERIOD_SEC_L1, &beacon.hibernation_mode_initial_time, beacon.second_counter);
        
        task_periodic(&init_radio, BEACON_RADIO_RESET_PERIOD_SEC, &beacon.last_radio_reset_time, beacon.second_counter);
        
        task_periodic(&beacon_reset, BEACON_SYSTEM_RESET_PERIOD_SEC, &beacon.last_system_reset_time, beacon.second_counter);
        
        beacon_enter_low_power_mode();      // Wait until the time timer execution (When the system leaves low-power mode)
        
    #if BEACON_MODE != DEBUG_MODE
        watchdog_reset_timer();
    #endif // BEACON_MODE
    }
}

void beacon_deinit()
{
    
}

void beacon_shutdown()
{
    //_BIS_SR(LPM4_5_bits);
}

void beacon_reset()
{
    //beacon_save_time();
    
    //WTDCTL = 0xDEAD;                // Reset system by writing to the WDT register without using the proper password
    
    PMMCTL0 = PMMPW | PMMSWBOR;     // Triggers a software BOR
}

void beacon_enter_hibernation()
{
    if (beacon.flags.hibernation == false)
    {
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("Entering in hibernation mode... ");
    #endif // DEBUG_MODE
        
        radio_sleep();
        
        beacon.flags.hibernation = true;
        beacon.obdh.enter_hibernation = false;
        
        beacon.hibernation_mode_initial_time = beacon.second_counter;
        
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
    
    beacon.flags.hibernation = false;
    
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
}

void beacon_enter_low_power_mode()
{
    _BIS_SR(LPM1_bits + GIE);
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

void beacon_generate_packet_payload()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Generating packet payload from ");
#endif // DEBUG_MODE

    uint8_t pkt_payload_counter = 0;

#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_SAT_ID
    uint8_t i = 0;
    for(i=0; i<sizeof(PKT_PAYLOAD_SAT_ID)-1; i++)
    {
        beacon.packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_ID[i];
    }
#endif // PAYLOAD_SAT_ID
    
    if ((beacon.obdh.crc_fails == 0) && (beacon.obdh.is_dead == false))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_OBDH_DATA
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("OBDH data... ");
    #endif // DEBUG_MODE
        // Battery 1 Voltage
        for(i=0; i<OBDH_COM_BAT1_VOLTAGE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.bat1_voltage[i];
        }
        
        // Battery 2 Voltage
        for(i=0; i<OBDH_COM_BAT2_VOLTAGE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.bat2_voltage[i];
        }
        
        // Battery 1 Temperature
        for(i=0; i<OBDH_COM_BAT1_TEMPERATURE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.bat1_temperature[i];
        }
        
        // Battery 2 Temperature
        for(i=0; i<OBDH_COM_BAT2_TEMPERATURE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.bat2_temperature[i];
        }
        
        // Total Charge of Batteries
        for(i=0; i<OBDH_COM_BAT_CHARGE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.bat_charge[i];
        }
        
        // Solar Panels Currents
        for(i=0; i<OBDH_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.solar_panels_currents[i];
        }
        
        // Solar Panels Voltages
        for(i=0; i<OBDH_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.solar_panels_voltages[i];
        }
        
        // Satellite Status
        for(i=0; i<OBDH_COM_SAT_STATUS_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.sat_status[i];
        }
        
        // IMU Data
        for(i=0; i<OBDH_COM_IMU_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.imu[i];
        }
        
        // System Time
        for(i=0; i<OBDH_COM_SYSTEM_TIME_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.system_time[i];
        }
        
        // OBDH Reset Counter
        for(i=0; i<OBDH_COM_RESET_COUNTER_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.obdh.data.reset_counter[i];
        }
#endif // PAYLOAD_OBDH_DATA
    }
    else if ((beacon.eps.crc_fails == 0) && (beacon.eps.is_dead == false))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_EPS_DATA
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("EPS data... ");
    #endif // DEBUG_MODE
        // Battery 1 Voltage
        for(i=0; i<EPS_COM_BAT1_VOLTAGE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.bat1_voltage[i];
        }
        
        // Battery 2 Voltage
        for(i=0; i<EPS_COM_BAT2_VOLTAGE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.bat2_voltage[i];
        }
        
        // Battery 1 Temperature
        for(i=0; i<EPS_COM_BAT1_TEMPERATURE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.bat1_temperature[i];
        }
        
        // Battery 2 Temperature
        for(i=0; i<EPS_COM_BAT2_TEMPERATURE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.bat2_temperature[i];
        }
        
        // Total Charge of Batteries
        for(i=0; i<EPS_COM_BAT_CHARGE_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.bat_charge[i];
        }
        
        // Solar Panels Currents
        for(i=0; i<EPS_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.solar_panels_currents[i];
        }
        
        // Solar Panels Voltages
        for(i=0; i<EPS_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
        {
            beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.solar_panels_voltages[i];
        }
        
        // Satellite Energy Level
        beacon.packet_payload.payload[pkt_payload_counter++] = beacon.eps.data.energy_level;
#endif // PAYLOAD_EPS_DATA
    }
    else
    {
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("no data... ");
#endif // DEBUG_MODE
    }
    
    beacon.packet_payload.length = pkt_payload_counter;
    
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
}

void beacon_generate_packets(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len, uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len)
{
    beacon_generate_packet_payload();
    
    NGHam_TX_Packet ngham_packet;
    ngham_tx_pkt_gen(&ngham_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ngham_encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);
    
    AX25_Packet ax25_packet;    
    ax25_beacon_pkt_gen(&ax25_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ax25_encode(&ax25_packet, ax25_pkt_str, ax25_pkt_str_len);
}

void beacon_generate_ngham_packet(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len)
{
    beacon_generate_packet_payload();
    
    NGHam_TX_Packet ngham_packet;
    
    ngham_tx_pkt_gen(&ngham_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ngham_encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);
}

void beacon_generate_ax25_packet(uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len)
{
    beacon_generate_packet_payload();
    
    AX25_Packet ax25_packet;
    
    ax25_beacon_pkt_gen(&ax25_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ax25_encode(&ax25_packet, ax25_pkt_str, ax25_pkt_str_len);
}

void beacon_transmit_ngham_packet()
{
    if (beacon.flags.hibernation == false)
    {
        if (beacon.flags.can_transmit == true)
        {
            beacon.flags.transmitting = true;

            uint8_t ngham_pkt_str[256];
            uint16_t ngham_pkt_str_len;

            beacon_generate_ngham_packet(ngham_pkt_str, &ngham_pkt_str_len);

        #if BEACON_RF_SWITCH != HW_NONE
            rf_switch_enable_beacon();
        #endif // BEACON_RF_SWITCH

        #if BEACON_PA != HW_NONE
            pa_enable();
        #endif // BEACON_PA
                
            radio_write_data(ngham_pkt_str+8, ngham_pkt_str_len-8);    // 8: Removing preamble and sync word from the NGHam packet

        #if BEACON_PA != HW_NONE
            pa_disable();
        #endif // BEACON_PA

        #if BEACON_RF_SWITCH != HW_NONE
            rf_switch_disable_beacon();
        #endif // BEACON_RF_SWITCH

            beacon.flags.transmitting = false;
        }
    }
}

void beacon_transmit_ax25_packet()
{
    if (beacon.flags.hibernation == false)
    {
        if (beacon.flags.can_transmit == true)
        {
            beacon.flags.transmitting = true;
            
            uint8_t ax25_pkt_str[256];
            uint16_t ax25_pkt_str_len;
            
            beacon_generate_ax25_packet(ax25_pkt_str, &ax25_pkt_str_len);
            
        #if BEACON_RF_SWITCH != HW_NONE
            rf_switch_enable_beacon();
        #endif // BEACON_RF_SWITCH

        #if BEACON_PA != HW_NONE
            pa_enable();
        #endif // BEACON_PA
            
            radio_write_data(ax25_pkt_str, ax25_pkt_str_len);
            
        #if BEACON_PA != HW_NONE
            pa_disable();
        #endif // BEACON_PA

        #if BEACON_RF_SWITCH != HW_NONE
            rf_switch_disable_beacon();
        #endif // BEACON_RF_SWITCH
            
            beacon.flags.transmitting = false;
        }
    }
}

void beacon_set_energy_level()
{
    if ((beacon.obdh.crc_fails) == 0 && (beacon.obdh.is_dead == false))
    {
        beacon.energy_level = beacon.obdh.data.sat_status[0];
    }
    else if ((beacon.eps.crc_fails == 0) && (beacon.eps.is_dead == false))
    {
        beacon.energy_level = beacon.eps.data.energy_level;
    }
    else
    {
        beacon.energy_level = SATELLITE_ENERGY_LEVEL_5;
    }
}

void beacon_check_devices_status()
{
    if ((beacon.second_counter - beacon.obdh.time_last_valid_pkt) <= OBDH_COM_DEADLINE_SEC)
    {
        beacon.obdh.is_dead = false;
    }
    else
    {
        beacon.obdh.is_dead = true;
    }
    
    if ((beacon.second_counter - beacon.eps.time_last_valid_pkt) <= EPS_COM_DEADLINE_SEC)
    {
        beacon.eps.is_dead = false;
    }
    else
    {
        beacon.eps.is_dead = true;
    }
    
    /*if ((beacon.obdh.is_dead == true) && (beacon.eps.is_dead = true))
    {
        
    }*/
    
    // Antenna connection status
    // Radio status
}

void beacon_process_received_packet_data(uint8_t *data, uint8_t len)
{
    if (beacon.flags.hibernation == false)
    {
        if ((data[6] == 's') && (data[7] == 'd'))
        {
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("Shutdown command received from ");
            debug_print_byte(data[0]);
            debug_print_byte(data[1]);
            debug_print_byte(data[2]);
            debug_print_byte(data[3]);
            debug_print_byte(data[4]);
            debug_print_byte(data[5]);
            debug_print_msg("!\n");
#endif // DEBUG_MODE
            
            uint8_t ngham_pkt_str[100];
            uint16_t ngham_pkt_str_len;
            
            uint8_t pkt_payload_len = 0;
            uint8_t pkt_payload[60];
            
            uint8_t shutdown_ack_start[] = "Shutdown received from ";
            uint8_t i = 0;
            for(i=0; i<sizeof(shutdown_ack_start); i++)
            {
                pkt_payload[pkt_payload_len++] = shutdown_ack_start[i];
            }
            
            pkt_payload[pkt_payload_len++] = data[0];
            pkt_payload[pkt_payload_len++] = data[1];
            pkt_payload[pkt_payload_len++] = data[2];
            pkt_payload[pkt_payload_len++] = data[3];
            pkt_payload[pkt_payload_len++] = data[4];
            pkt_payload[pkt_payload_len++] = data[5];
            
            uint8_t shutdown_ack_end[] = ". Wake up time in 24 hours.";
            for(i=0; i<sizeof(shutdown_ack_end); i++)
            {
                pkt_payload[pkt_payload_len++] = shutdown_ack_end[i];
            }
            
            NGHam_TX_Packet ngham_packet;
            ngham_tx_pkt_gen(&ngham_packet, pkt_payload, pkt_payload_len);
            ngham_encode(&ngham_packet, ngham_pkt_str, &ngham_pkt_str_len);
            
            while(beacon.flags.can_transmit != true)
            {
                if (beacon.flags.can_transmit == true)
                {
                    radio_write_data(ngham_pkt_str, ngham_pkt_str_len);
                }
            }
            
            beacon.obdh.enter_hibernation = true;
        }
    }
}

void beacon_antenna_deployment()
{
    uint8_t time_marker = beacon.second_counter;
    
    while((beacon.second_counter - time_marker) <= BEACON_ANTENNA_DEPLOY_SLEEP_SEC)
    {
#if BEACON_MODE != DEBUG_MODE
        watchdog_reset_timer();
#endif // BEACON_MODE
        
        beacon_enter_low_power_mode();
    }
    
    antenna_deploy();
}

void beacon_receive_packet(uint8_t *pkt, uint8_t len)
{
    uint8_t pkt_payload[100];
    uint8_t pkt_payload_len;
    
    uint8_t i = 0;
    for(i=0; i<len; i++)
    {
        uint8_t decoder_state = ngham_decode(pkt[i], pkt_payload, &pkt_payload_len);
        if (decoder_state == PKT_CONDITION_OK)
        {
            beacon_process_received_packet_data(pkt_payload, pkt_payload_len);
            break;
        }
        else if (decoder_state == PKT_CONDITION_PREFAIL)
        {
            continue;
        }
        else if (decoder_state == PKT_CONDITION_FAIL)
        {
            break;
        }
    }
}

//! \} End of beacon group
