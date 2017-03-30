/*
 * ax25.c
 * 
 * Copyright (C) 2016, Universidade Federal de Santa Catarina
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file ax25.c
 * 
 * \brief AX25 functions implementation
 * 
 * This module have all the functions to generate/manipulate packets with
 * the AX25 protocol.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 16/12/2016
 *
 * \addtogroup ax25
 * \{
 */

#include "../inc/ax25.h"
#include "../inc/crc.h"
#include "../inc/debug.h"

void ax25_BeaconPacketGen(AX25_Packet *ax25_packet, uint8_t *data, uint16_t data_size)
{
#if DEBUG_MODE == true
    debug_PrintMsg("Generating AX25 packet... ");
#endif // DEBUG_MODE

    AX25_Transfer_Frame_Header destination;
    AX25_Transfer_Frame_Header source;
    
    uint8_t dst_callsign[] = AX25_FLORIPASAT_CALLSIGN;
    uint8_t src_callsign[] = AX25_FLORIPASAT_CALLSIGN;
    
    uint8_t i = 0;
    for(i=0;i<7;i++)
    {
        destination.callsign[i] = dst_callsign[i] << 1;
    }
    destination.ssid = AX25_ADR_NON_REPEATER_SSID | (uint8_t)(AX25_FLORIPASAT_GRS_SSID << 1) | AX25_ADR_DST_COMMAND;
    
    for(i=0;i<7;i++)
    {
        source.callsign[i] = src_callsign[i] << 1;
    }
    source.ssid = AX25_ADR_NON_REPEATER_SSID | (uint8_t)(AX25_FLORIPASAT_SAT_SSID << 1) | AX25_ADR_SRC_COMMAND;
    
    ax25_packet->start_flag      = AX25_FLAG;
    ax25_packet->dst_adr         = destination;
    ax25_packet->src_adr         = source;
    ax25_packet->control_bits    = AX25_CTRL_UNNUMBERED_UI | AX25_CTRL_PF_DISABLE;
    ax25_packet->protocol_id     = AX25_PID_NO_LAYER_3;
    for(i=0;i<data_size;i++)
    {
        ax25_packet->data[i] = data[i];
    }
    ax25_packet->fcs             = crc16_CCITT(0x0000, data, data_size);
    ax25_packet->end_flag        = AX25_FLAG;
    
#if DEBUG_MODE == true
    debug_PrintMsg("DONE!\n");
#endif // DEBUG_MODE
}

void ax25_UpdateDataFromPacket(AX25_Packet *ax25_packet, uint8_t *new_data, uint16_t new_data_size)
{
#if DEBUG_MODE == true
    debug_PrintMsg("Updating an existing AX25 packet... ");
#endif // DEBUG_MODE

    if (new_data_size > 256)
    {
        new_data_size = 256;
    }
    
    uint8_t i = 0;
    for(i=0;i<new_data_size;i++)
    {
        ax25_packet->data[i] = new_data[i];
    }
    
    ax25_packet->fcs = crc16_CCITT(0x0000, new_data, new_data_size);

#if DEBUG_MODE == true
    debug_PrintMsg("DONE!\n");
#endif // DEBUG_MODE
}

void ax25_Packet2String(AX25_Packet *ax25_packet, uint8_t *str_packet, uint16_t data_size)
{
    str_packet[0] = ax25_packet->start_flag;
    
    uint8_t i = 0;
    for(i=0;i<7;i++)
        str_packet[i+1] = ax25_packet->dst_adr.callsign[i];
    
    str_packet[7] = ax25_packet->dst_adr.ssid;
    
    for(i=0;i<7;i++)
        str_packet[i+8] = ax25_packet->src_adr.callsign[i];
    
    str_packet[14] = ax25_packet->src_adr.ssid;
    str_packet[15] = ax25_packet->control_bits;
    str_packet[16] = ax25_packet->protocol_id;
    
    for(i=0;i<data_size;i++)
    {
        str_packet[i+17] = ax25_packet->data[i];
    }
    
    str_packet[16 + data_size + 1] = (uint8_t)((ax25_packet->fcs & 0xFF00) >> 8);    // CRC16 MSB
    str_packet[16 + data_size + 2] = (uint8_t)(ax25_packet->fcs & 0x00FF);           // CRC16 LSB
    str_packet[16 + data_size + 3] = ax25_packet->end_flag;
}

//! \} End of ax25 implementation group
