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

void task_transmit_ngham_packet(Beacon *beacon_ptr)
{
    beacon_ptr->flags.transmitting = true;
    
    uint8_t ngham_pkt_str[256];
    uint16_t ngham_pkt_str_len;
    
    task_generate_ngham_packet(ngham_pkt_str, &ngham_pkt_str_len);
    
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

    beacon_ptr->flags.transmitting = false;
}

void task_transmit_ax25_packet(Beacon *beacon_ptr)
{
    beacon_ptr->flags.transmitting = true;
    
    uint8_t ax25_pkt_str[256];
    uint16_t ax25_pkt_str_len;
    
    task_generate_ax25_packet(ax25_pkt_str, &ax25_pkt_str_len);
    
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
    
    beacon_ptr->flags.transmitting = false;
}

void task_receive_packet(uint8_t *pkt, uint8_t len)
{
    uint8_t pkt_payload[100];
    uint8_t pkt_payload_len;
    
    uint8_t i = 0;
    for(i=0; i<len; i++)
    {
        uint8_t decoder_state = ngham_decode(pkt[i], pkt_payload, &pkt_payload_len);
        if (decoder_state == PKT_CONDITION_OK)
        {
            task_process_received_packet_data(pkt_payload, pkt_payload_len);
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

void task_process_received_packet_data(uint8_t *data, uint8_t len)
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
            
            task_enter_hibernation();
        }
    }
}

void task_generate_packets(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len, uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len)
{    
    task_generate_packet_payload(&beacon);
    
    NGHam_TX_Packet ngham_packet;
    ngham_tx_pkt_gen(&ngham_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ngham_encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);

    AX25_Packet ax25_packet;    
    ax25_beacon_pkt_gen(&ax25_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ax25_encode(&ax25_packet, ax25_pkt_str, ax25_pkt_str_len);
}

void task_generate_ngham_packet(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len)
{
    task_generate_packet_payload(&beacon);
    
    NGHam_TX_Packet ngham_packet;
    
    ngham_tx_pkt_gen(&ngham_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ngham_encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);
}

void task_generate_ax25_packet(uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len)
{
    task_generate_packet_payload(&beacon);
    
    AX25_Packet ax25_packet;
    
    ax25_beacon_pkt_gen(&ax25_packet, beacon.packet_payload.payload, beacon.packet_payload.length);
    ax25_encode(&ax25_packet, ax25_pkt_str, ax25_pkt_str_len);
}

void task_enter_low_power_mode()
{
    _BIS_SR(LPM1_bits + GIE);
}

void task_leave_low_power_mode()
{
    //_BIC_SR(LPM1_EXIT);
}

void task_enter_hibernation()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Entering in hibernation mode... ");
#endif // DEBUG_MODE
    
    radio_sleep();
    
    beacon.flags.hibernation = true;
    
    beacon.hibernation_mode_initial_time = beacon.second_counter;
    
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
    uint8_t time_marker = beacon.second_counter;
    
    while((beacon.second_counter - time_marker) <= BEACON_ANTENNA_DEPLOY_SLEEP_SEC)
    {
#if BEACON_MODE != DEBUG_MODE
        watchdog_reset_timer();
#endif // BEACON_MODE
        
        task_enter_low_power_mode();
    }
    
    antenna_deploy();
}

void task_set_energy_level(Beacon *beacon_ptr)
{
    if ((beacon_ptr->obdh.crc_fails) == 0 && (beacon_ptr->obdh.is_dead == false))
    {
        beacon_ptr->energy_level = beacon_ptr->obdh.data.sat_status[0];
    }
    else if ((beacon_ptr->eps.crc_fails == 0) && (beacon_ptr->eps.is_dead == false))
    {
        beacon_ptr->energy_level = beacon_ptr->eps.data.energy_level;
    }
    else
    {
        beacon_ptr->energy_level = SATELLITE_ENERGY_LEVEL_5;
    }
}

uint8_t task_get_tx_period(Beacon *beacon_ptr)
{
    switch(beacon_ptr->energy_level)
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

void task_generate_packet_payload(Beacon *beacon_ptr)
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Generating packet payload from ");
#endif // DEBUG_MODE

    uint8_t pkt_payload_counter = 0;

#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_SAT_ID
    uint8_t i = 0;
    for(i=0; i<sizeof(PKT_PAYLOAD_SAT_ID)-1; i++)
    {
        beacon_ptr->packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_ID[i];
    }
#endif // PAYLOAD_SAT_ID
    
    if ((beacon_ptr->obdh.crc_fails == 0) && (beacon_ptr->obdh.is_dead == false))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_OBDH_DATA
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("OBDH data... ");
    #endif // DEBUG_MODE
        // Battery 1 Voltage
        for(i=0; i<OBDH_COM_BAT1_VOLTAGE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.bat1_voltage[i];
        }
        
        // Battery 2 Voltage
        for(i=0; i<OBDH_COM_BAT2_VOLTAGE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.bat2_voltage[i];
        }
        
        // Battery 1 Temperature
        for(i=0; i<OBDH_COM_BAT1_TEMPERATURE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.bat1_temperature[i];
        }
        
        // Battery 2 Temperature
        for(i=0; i<OBDH_COM_BAT2_TEMPERATURE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.bat2_temperature[i];
        }
        
        // Total Charge of Batteries
        for(i=0; i<OBDH_COM_BAT_CHARGE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.bat_charge[i];
        }
        
        // Solar Panels Currents
        for(i=0; i<OBDH_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.solar_panels_currents[i];
        }
        
        // Solar Panels Voltages
        for(i=0; i<OBDH_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.solar_panels_voltages[i];
        }
        
        // Satellite Status
        for(i=0; i<OBDH_COM_SAT_STATUS_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.sat_status[i];
        }
        
        // IMU Data
        for(i=0; i<OBDH_COM_IMU_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.imu[i];
        }
        
        // System Time
        for(i=0; i<OBDH_COM_SYSTEM_TIME_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.system_time[i];
        }
        
        // OBDH Reset Counter
        for(i=0; i<OBDH_COM_RESET_COUNTER_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->obdh.data.reset_counter[i];
        }
#endif // PAYLOAD_OBDH_DATA
    }
    else if ((beacon_ptr->eps.crc_fails == 0) && (beacon_ptr->eps.is_dead == false))
    {
#if BEACON_PACKET_PAYLOAD_CONTENT & PAYLOAD_EPS_DATA
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("EPS data... ");
    #endif // DEBUG_MODE
        // Battery 1 Voltage
        for(i=0; i<EPS_COM_BAT1_VOLTAGE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.bat1_voltage[i];
        }
        
        // Battery 2 Voltage
        for(i=0; i<EPS_COM_BAT2_VOLTAGE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.bat2_voltage[i];
        }
        
        // Battery 1 Temperature
        for(i=0; i<EPS_COM_BAT1_TEMPERATURE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.bat1_temperature[i];
        }
        
        // Battery 2 Temperature
        for(i=0; i<EPS_COM_BAT2_TEMPERATURE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.bat2_temperature[i];
        }
        
        // Total Charge of Batteries
        for(i=0; i<EPS_COM_BAT_CHARGE_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.bat_charge[i];
        }
        
        // Solar Panels Currents
        for(i=0; i<EPS_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.solar_panels_currents[i];
        }
        
        // Solar Panels Voltages
        for(i=0; i<EPS_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
        {
            beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.solar_panels_voltages[i];
        }
        
        // Satellite Energy Level
        beacon_ptr->packet_payload.payload[pkt_payload_counter++] = beacon_ptr->eps.data.energy_level;
#endif // PAYLOAD_EPS_DATA
    }
    else
    {
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("no data... ");
#endif // DEBUG_MODE
    }
    
    beacon_ptr->packet_payload.length = pkt_payload_counter;
    
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
}

void task_enable_rx()
{
    radio_init_rx_isr();
    radio_enable_rx();
}

void task_disable_rx()
{
    radio_sleep();
}


void task_reset_radio(Beacon *beacon_ptr)
{
    if ((beacon_ptr->second_counter - beacon_ptr->last_radio_reset_time) >= BEACON_RADIO_RESET_PERIOD_SEC)
    {
        radio_reset();
        
        beacon_ptr->last_radio_reset_time = beacon_ptr->second_counter;
    }
}

//! \} End of tasks group
