/*
 * ax25.c
 * 
 * Copyright (C) 2016-2019, Universidade Federal de Santa Catarina.
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
 * \brief AX25 functions implementation
 * 
 * This module have all the functions to generate/manipulate packets with the AX25 protocol.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.2
 * 
 * \date 16/12/2016
 *
 * \addtogroup ax25
 * \{
 */

#include <config/config.h>
#include <src/crc/crc.h>
#include <system/debug/debug.h>

#include "ax25.h"

void ax25_beacon_pkt_gen(AX25_Packet *ax25_packet, uint8_t *data, uint16_t data_size)
{
    debug_print_event_from_module(DEBUG_INFO, AX25_MODULE_NAME, "Generating AX25 packet...\n\r");

    AX25_Transfer_Frame_Header destination;
    AX25_Transfer_Frame_Header source;
    
    uint8_t dst_callsign[] = AX25_DESTINATION_CALLSIGN;
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
    
    ax25_packet->start_flag     = AX25_FLAG;
    ax25_packet->dst_adr        = destination;
    ax25_packet->src_adr        = source;
    ax25_packet->control_bits   = AX25_CTRL_UNNUMBERED_UI | AX25_CTRL_PF_DISABLE;
    ax25_packet->protocol_id    = AX25_PID_NO_LAYER_3;
    for(i=0;i<data_size;i++)
    {
        ax25_packet->payload.data[i] = data[i];
    }
    ax25_packet->payload.len    = data_size;
    ax25_packet->fcs            = crc16_CCITT(0x0000, data, data_size);
    ax25_packet->end_flag       = AX25_FLAG;
}

void ax25_update_data_from_pkt(AX25_Packet *ax25_packet, uint8_t *new_data, uint16_t new_data_size)
{
    debug_print_event_from_module(DEBUG_INFO, AX25_MODULE_NAME, "Updating an existing AX25 packet...\n\r");

    if (new_data_size > 256)
    {
        new_data_size = 256;
    }
    
    uint8_t i = 0;
    for(i=0;i<new_data_size;i++)
    {
        ax25_packet->payload.data[i] = new_data[i];
    }
    ax25_packet->payload.len = new_data_size;
    
    ax25_packet->fcs = crc16_CCITT(0x0000, new_data, new_data_size);
}

void ax25_pkt_2_str(AX25_Packet *ax25_packet, uint8_t *str_pkt, uint16_t *str_pkt_len)
{
    *str_pkt_len = 0;
    
    str_pkt[(*str_pkt_len)++] = ax25_packet->start_flag;
    
    uint8_t i = 0;
    for(i=0; i<7; i++)
    {
        str_pkt[(*str_pkt_len)++] = ax25_packet->dst_adr.callsign[i];
    }
    
    str_pkt[(*str_pkt_len)++] = ax25_packet->dst_adr.ssid;
    
    for(i=0; i<7; i++)
    {
        str_pkt[(*str_pkt_len)++] = ax25_packet->src_adr.callsign[i];
    }
    
    str_pkt[(*str_pkt_len)++] = ax25_packet->src_adr.ssid;
    str_pkt[(*str_pkt_len)++] = ax25_packet->control_bits;
    str_pkt[(*str_pkt_len)++] = ax25_packet->protocol_id;
    
    for(i=0; i<ax25_packet->payload.len; i++)
    {
        str_pkt[(*str_pkt_len)++] = ax25_packet->payload.data[i];
    }
    
    str_pkt[(*str_pkt_len)++] = (uint8_t)((ax25_packet->fcs & 0xFF00) >> 8);  // CRC16 MSB
    str_pkt[(*str_pkt_len)++] = (uint8_t)(ax25_packet->fcs & 0x00FF);         // CRC16 LSB
    str_pkt[(*str_pkt_len)++] = ax25_packet->end_flag;
}

void ax25_bit_stuffing(uint8_t *pkt, uint16_t pkt_len, uint8_t *new_pkt, uint16_t *new_pkt_len)
{
    // STEP 1: Byte packet to bit packet
    bit bit_pkt[8*(21+256)];        // 21 = header, 256 = payload, 8 = byte to bit conversion
    
    uint16_t bit_pkt_counter = 0;
    uint16_t i = 0;
    for(i=0; i<pkt_len; i++)
    {
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x01) == 0x01? 1:0;
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x02) == 0x02? 1:0;
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x04) == 0x04? 1:0;
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x08) == 0x08? 1:0;
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x10) == 0x10? 1:0;
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x20) == 0x20? 1:0;
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x40) == 0x40? 1:0;
        bit_pkt[bit_pkt_counter++].val = (pkt[i] & 0x80) == 0x80? 1:0;
    }
    
    // STEP 2: Bit stuffing
    bit bit_pkt_new[8*(21+256)+50];
    uint16_t bit_pkt_new_counter = 0;
    
    bit bit_buffer[5];
    uint8_t bit_buffer_counter = 0;
    for(i=0; i<bit_pkt_counter; i++)
    {
        bit_buffer[bit_buffer_counter++].val = bit_pkt[i].val;
        bit_pkt_new[bit_pkt_new_counter++].val = bit_pkt[i].val;
        
        if (bit_buffer_counter == 5)
        {
            if (bit_buffer[0].val & bit_buffer[1].val & bit_buffer[2].val & bit_buffer[3].val & bit_buffer[4].val)
            {
                bit_pkt_new[bit_pkt_new_counter++].val = 0;
                bit_buffer_counter = 0;
            }
            else
            {
                bit_buffer[0] = bit_buffer[1];
                bit_buffer[1] = bit_buffer[2];
                bit_buffer[2] = bit_buffer[3];
                bit_buffer[3] = bit_buffer[4];
                bit_buffer_counter--;
            }
        }
    }
    
    // STEP 3: Bit packet to byte packet
    bit byte_buffer[8];
    uint8_t byte_buffer_counter = 0;
    *new_pkt_len = 0;
    
    for(i=0; i<bit_pkt_new_counter; i++)
    {
        byte_buffer[byte_buffer_counter++] = bit_pkt_new[i];
        
        if (((bit_pkt_new_counter - i) < 8) && ((bit_pkt_new_counter % 8) != 0) && (byte_buffer_counter == 1))
        {
            new_pkt[*new_pkt_len] = 0x00;
            uint8_t j = 0;
            for(j=0; j<(bit_pkt_new_counter-i); j++)
            {
                new_pkt[*new_pkt_len] |= (bit_pkt_new[i+j].val << (7-j));
            }
            
            (*new_pkt_len)++;
            
            break;
        }
        
        if (byte_buffer_counter == 8)
        {
            new_pkt[(*new_pkt_len)++] = (byte_buffer[0].val << 7) | (byte_buffer[1].val << 6) | (byte_buffer[2].val << 5) |
                                        (byte_buffer[3].val << 4) | (byte_buffer[4].val << 3) | (byte_buffer[5].val << 2) |
                                        (byte_buffer[6].val << 1) | byte_buffer[7].val;
            byte_buffer_counter = 0;
        }
    }
}

void ax25_encode(AX25_Packet *ax25_pkt, uint8_t *pkt, uint16_t *pkt_len)
{
    uint8_t pkt_str[256+21];
    uint16_t pkt_str_len;
    
    ax25_pkt_2_str(ax25_pkt, pkt_str, &pkt_str_len);
    
    ax25_bit_stuffing(pkt_str, pkt_str_len, pkt, pkt_len);
}

//! \} End of ax25 implementation group
