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

#include <config/config.h>
#include <modules/modules.h>
#include <libs/libs.h>

#include "tasks.h"
#include "flags.h"

void task_transmit_packet()
{
    uint8_t ngham_pkt_str[256];
    uint8_t ngham_pkt_str_len;
    uint8_t ax25_pkt_str[256];
    uint8_t ax25_pkt_str_len;
    
    task_generate_packets(ngham_pkt_str, &ngham_pkt_str_len, ax25_pkt_str, &ax25_pkt_str_len);
    
#if BEACON_RF_SWITCH != HW_NONE
    rf_switch_enable_beacon();
#endif // BEACON_RF_SWITCH

#if BEACON_PA != HW_NONE
    pa_enable();
#endif // BEACON_PA
    
    radio_write_data(ngham_pkt_str, ngham_pkt_str_len);

#if BEACON_PA != HW_NONE
    pa_disable();
#endif // BEACON_PA

#if BEACON_RF_SWITCH != HW_NONE
    rf_switch_disable_beacon();
#endif // BEACON_RF_SWITCH
    
    // Wait for AX25 packet transmission to end
    delay_ms((uint16_t)(1.2*sizeof(ax25_str_pkt)*1000/1200/8));
    
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
}

void task_generate_packets(uint8_t *ngham_pkt_str, uint8_t *ngham_pkt_str_len, uint8_t *ax25_pkt_str, uint8_t *ax25_pkt_str_len)
{
    NGHam_TX_Packet ngham_packet;
    AX25_Packet ax25_packet;
    
    packet_payload_generate(packet_payload, eps_data);
    
    ngham_TxPktGen(&ngham_packet, packet_payload.payload, packet_payload.lenght);
    ngham_Encode(&ngham_packet, ngham_pkt_str, ngham_pkt_str_len);
    
    ax25_BeaconPacketGen(&ax25_packet, packet_payload.payload, packet_payload.lenght);
    ax25_Packet2String(&ax25_packet, ax25_pkt_str, ax25_pkt_str_len);
}

void task_enter_low_power_mode()
{
    _BIS_SR(LPM1_bits + GIE);
}

void task_leave_low_power_mode()
{
    _BIC_SR(LPM1_EXIT);
}

uint16_t task_check_elapsed_time(uint16_t initial_time, uint16_t final_time, uint8_t time_unit)
{
    switch(time_unit)
    {
        case MILLISECONDS:
            return (uint16_t)((final_time - initial_time) % 1000);
            break;
        case SECONDS:
            return (uint16_t)((final_time - initial_time) % 60);
            break;
        case MINUTES:
            return (uint16_t)((final_time - initial_time) % 60);
            break;
        case HOURS:
            return (uint16_t)((final_time - initial_time) % 24);
            break;
        case DAYS:
            return (uint16_t)((final_time - initial_time) % 7);
            break;
        case WEEKS:
            return (uint16_t)((final_time - initial_time) % 4);
            break;
        case MONTHS:
            return (uint16_t)((final_time - initial_time) % 12);
            break;
        case YEARS:
            return final_time - initial_time;
            break;
        default:
            return 0x00;
    }
}

void task_enter_hibernation()
{
    radio_sleep();
    
    flags.hibernation = true;
}

void task_leave_hibernation()
{
    radio_wake_up();
    
    flags.hibernation = false;
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
    uint8_t minute_marker = time.minute;
    
    while(task_check_elapsed_time(minute_marker, time.minute, MINUTES) <= BEACON_ANTENNA_DEPLOY_SLEEP_MIN)
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

//! \} End of tasks group
