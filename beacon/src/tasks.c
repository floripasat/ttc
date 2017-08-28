/*
 * tasks.c
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
 * \file tasks.h
 * 
 * \brief Beacon tasks implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \addtogroup tasks
 * \{
 */

#include <string.h>

#include <config/config.h>
#include <modules/modules.h>
#include <libs/libs.h>

#include "tasks.h"
#include "beacon.h"
#include "flags.h"

void task_transmit_packet(Beacon *beacon_ptr)
{
    beacon_ptr->flags.transmitting = true;
    
    uint8_t ngham_pkt_str[256];
    uint16_t ngham_pkt_str_len;
    uint8_t ax25_pkt_str[256];
    uint16_t ax25_pkt_str_len;
    
    task_generate_packets(ngham_pkt_str, &ngham_pkt_str_len, ax25_pkt_str, &ax25_pkt_str_len);
    
#if BEACON_PACKET_PROTOCOL & PACKET_NGHAM
    #if BEACON_RF_SWITCH != HW_NONE
        rf_switch_enable_beacon();
    #endif // BEACON_RF_SWITCH

    #if BEACON_PA != HW_NONE
        pa_enable();
    #endif // BEACON_PA
        
        uint8_t buffer[128];
        uint8_t i = 0;
        for(i=0; i<ngham_pkt_str_len-8; i++)
        {
            buffer[i] = ngham_pkt_str[i+8];     // Removing preamble and sync word from the NGHam packet
        }
        
        //radio_write_data(ngham_pkt_str, ngham_pkt_str_len);
        radio_write_data(buffer, ngham_pkt_str_len-8);

    #if BEACON_PA != HW_NONE
        pa_disable();
    #endif // BEACON_PA

    #if BEACON_RF_SWITCH != HW_NONE
        rf_switch_disable_beacon();
    #endif // BEACON_RF_SWITCH
#endif // PACKET_NGHAM

#if BEACON_PACKET_PROTOCOL & PACKET_AX25
    #if BEACON_RF_SWITCH != HW_NONE
        rf_switch_enable_beacon();
    #endif // BEACON_RF_SWITCH

    #if BEACON_PA != HW_NONE
        pa_enable();
    #endif // BEACON_PA

    #if BEACON_PACKET_PROTOCOL & PACKET_AX25    
        radio_write_data(ax25_pkt_str, ax25_pkt_str_len);
    #endif // PACKET_AX25

    #if BEACON_PA != HW_NONE
        pa_disable();
    #endif // BEACON_PA

    #if BEACON_RF_SWITCH != HW_NONE
        rf_switch_disable_beacon();
    #endif // BEACON_RF_SWITCH
#endif // PACKET_AX25

    beacon_ptr->flags.transmitting = false;
}

void task_generate_packets(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len, uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len)
{    
    task_generate_packet_payload(&beacon);
    
#if BEACON_PACKET_PROTOCOL & PACKET_NGHAM
    NGHam_TX_Packet ngham_packet;
    ngham_TxPktGen(&ngham_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ngham_Encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);
#endif // PACKET_NGHAM

#if BEACON_PACKET_PROTOCOL & PACKET_AX25
    AX25_Packet ax25_packet;
    
    ax25_BeaconPacketGen(&ax25_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ax25_Packet2String(&ax25_packet, ax25_pkt_str, *ax25_pkt_str_len);
#endif // PACKET_AX25
}

void task_enter_low_power_mode()
{
    _BIS_SR(LPM1_bits + GIE);
}

void task_leave_low_power_mode()
{
    //_BIC_SR(LPM1_EXIT);
}

uint16_t task_check_elapsed_time(uint16_t initial_time, uint16_t final_time, uint8_t time_unit)
{
    switch(time_unit)
    {
        case MILLISECONDS:
            return (uint16_t)((final_time - initial_time) % 1000);
        case SECONDS:
            return (uint16_t)((final_time - initial_time) % 60);
        case MINUTES:
            return (uint16_t)((final_time - initial_time) % 60);
        case HOURS:
            return (uint16_t)((final_time - initial_time) % 24);
        case DAYS:
            return (uint16_t)((final_time - initial_time) % 7);
        case WEEKS:
            return (uint16_t)((final_time - initial_time) % 4);
        case MONTHS:
            return (uint16_t)((final_time - initial_time) % 12);
        case YEARS:
            return final_time - initial_time;
        default:
            return 0x00;
    }
}

void task_enter_hibernation()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Entering in hibernation mode... ");
#endif // DEBUG_MODE
    radio_sleep();
    
    beacon.flags.hibernation = true;
    
    beacon.hibernation_mode_initial_time.millisecond    = beacon.time.millisecond;
    beacon.hibernation_mode_initial_time.second         = beacon.time.second;
    beacon.hibernation_mode_initial_time.minute         = beacon.time.minute;
    beacon.hibernation_mode_initial_time.hour           = beacon.time.hour;
    beacon.hibernation_mode_initial_time.day            = beacon.time.day;
    beacon.hibernation_mode_initial_time.week           = beacon.time.week;
    beacon.hibernation_mode_initial_time.month          = beacon.time.month;
    beacon.hibernation_mode_initial_time.year           = beacon.time.year;
    
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
}

void task_leave_hibernation()
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

void task_save_time()
{
    
}

void task_load_time()
{
    
}

void task_reset_system()
{
    
}

void task_antenna_deployment()
{
    uint8_t minute_marker = beacon.time.minute;
    
    while(task_check_elapsed_time(minute_marker, beacon.time.minute, MINUTES) <= BEACON_ANTENNA_DEPLOY_SLEEP_MIN)
    {
#if BEACON_MODE != DEBUG_MODE
        watchdog_reset_timer();
#endif // BEACON_MODE
        
        task_enter_low_power_mode();
    }
    
    while(antenna_deploy() != STATUS_SUCCESS)
    {
        
    }
}

void task_set_energy_level(Beacon *beacon_ptr)
{
    if (beacon_ptr->obdh.crc_fails == 0)
    {
        beacon_ptr->energy_level = beacon_ptr->obdh.data.sat_status[0];
    }
    else if (beacon_ptr->eps.crc_fails == 0)
    {
        beacon_ptr->energy_level = beacon_ptr->eps.data.energy_level;
    }
    else
    {
        beacon_ptr->energy_level = SATELLITE_ENERGY_LEVEL_5;
    }
}

uint8_t task_get_tx_period()
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

void task_generate_packet_payload(Beacon *b)
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Generating packet payload from ");
#endif // DEBUG_MODE

    uint8_t pkt_payload_counter = 0;

#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_SAT_ID
    uint8_t i = 0;
    for(i=0; i<sizeof(PKT_PAYLOAD_SAT_ID)-1; i++)
    {
        b->packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_ID[i];
    }
#endif // PAYLOAD_SAT_ID
    
    if (b->obdh.is_dead == false)
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_OBDH_DATA
    #if BEACON_MODE == DEBUG_MODE
            debug_print_msg("OBDH data... ");
    #endif // DEBUG_MODE
            // Battery 1 Voltage
            for(i=0; i<OBDH_COM_BAT1_VOLTAGE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.bat1_voltage[i];
            }
            
            // Battery 2 Voltage
            for(i=0; i<OBDH_COM_BAT2_VOLTAGE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.bat2_voltage[i];
            }
            
            // Battery 1 Temperature
            for(i=0; i<OBDH_COM_BAT1_TEMPERATURE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.bat1_temperature[i];
            }
            
            // Battery 2 Temperature
            for(i=0; i<OBDH_COM_BAT2_TEMPERATURE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.bat2_temperature[i];
            }
            
            // Total Charge of Batteries
            for(i=0; i<OBDH_COM_BAT_CHARGE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.bat_charge[i];
            }
            
            // Solar Panels Currents
            for(i=0; i<OBDH_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.solar_panels_currents[i];
            }
            
            // Solar Panels Voltages
            for(i=0; i<OBDH_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.solar_panels_voltages[i];
            }
            
            // Satellite Status
            for(i=0; i<OBDH_COM_SAT_STATUS_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.sat_status[i];
            }
            
            // IMU Data
            for(i=0; i<OBDH_COM_IMU_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.imu[i];
            }
            
            // System Time
            for(i=0; i<OBDH_COM_SYSTEM_TIME_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.system_time[i];
            }
            
            // OBDH Reset Counter
            for(i=0; i<OBDH_COM_RESET_COUNTER_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->obdh.data.reset_counter[i];
            }
#endif // PAYLOAD_OBDH_DATA
    }
    else if ((b->eps.crc_fails == 0) && ((task_check_elapsed_time(b->eps.time_last_valid_pkt.minute, b->time.minute, MINUTES) < EPS_COM_DEADLINE_MIN)))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_EPS_DATA
    #if BEACON_MODE == DEBUG_MODE
            debug_print_msg("EPS data... ");
    #endif // DEBUG_MODE
            
            // Battery 1 Voltage
            for(i=0; i<EPS_COM_BAT1_VOLTAGE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.bat1_voltage[i];
            }
            
            // Battery 2 Voltage
            for(i=0; i<EPS_COM_BAT2_VOLTAGE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.bat2_voltage[i];
            }
            
            // Battery 1 Temperature
            for(i=0; i<EPS_COM_BAT1_TEMPERATURE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.bat1_temperature[i];
            }
            
            // Battery 2 Temperature
            for(i=0; i<EPS_COM_BAT2_TEMPERATURE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.bat2_temperature[i];
            }
            
            // Total Charge of Batteries
            for(i=0; i<EPS_COM_BAT_CHARGE_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.bat_charge[i];
            }
            
            // Solar Panels Currents
            for(i=0; i<EPS_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.solar_panels_currents[i];
            }
            
            // Solar Panels Voltages
            for(i=0; i<EPS_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
            {
                b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.solar_panels_voltages[i];
            }
            
            // Satellite Energy Level
            b->packet_payload.payload[pkt_payload_counter++] = b->eps.data.energy_level;
#endif // PAYLOAD_EPS_DATA
    }
    else
    {
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("no data... ");
#endif // DEBUG_MODE
    }
    
    b->packet_payload.length = pkt_payload_counter;
    
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
}

//! \} End of tasks group
