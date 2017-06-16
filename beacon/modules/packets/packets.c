/*
 * packets.h
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
 * \file packets.h
 * 
 * \brief Packets handling routines impĺementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 14/06/2017
 * 
 * \addtogroup packets
 * \{
 */

#include "packets.h"

PacketPayload packet_payload;

void packet_payload_generate(PacketPayload pkt_payload, EPSData eps_data)
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Generating packet payload... ");
#endif // DEBUG_MODE

    uint8_t i = 0;
    uint8_t pkt_payload_counter = 0;
    for(i=0; i<sizeof(PKT_PAYLOAD_SAT_ID)-1; i++)
    {
        packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_ID[i];
    }
    
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_BATT_SEPARATOR[0];
    
    for(i=0; i<sizeof(PKT_PAYLOAD_BATT1_ID)-1; i++)
    {
        packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT1_ID[i];
    }
    
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT_VALUE_SEPARATOR[0];
    
    uint8_t digits[4];
    packet_payload_extract_digits(0.004883*((eps_data.bat1_msb << 8) + eps_data.bat1_lsb), digits);
    
    packet_payload.payload[pkt_payload_counter++] = digits[0];
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_DECIMAL_SEPARATOR;
    packet_payload.payload[pkt_payload_counter++] = digits[1];
    packet_payload.payload[pkt_payload_counter++] = digits[2];
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_VOLTAGE_UNIT;
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_BATT_SEPARATOR[0];
    
    for(i=0; i<sizeof(PKT_PAYLOAD_BATT2_ID)-1; i++)
    {
        packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT2_ID[i];
    }
    
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT_VALUE_SEPARATOR[0];
    
    packet_payload_extract_digits(0.004883*((eps_data.bat2_msb << 8) + eps_data.bat2_lsb), digits);
    
    packet_payload.payload[pkt_payload_counter++] = digits[0];
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_DECIMAL_SEPARATOR;
    packet_payload.payload[pkt_payload_counter++] = digits[1];
    packet_payload.payload[pkt_payload_counter++] = digits[2];
    packet_payload.payload[pkt_payload_counter++] = PKT_PAYLOAD_VOLTAGE_UNIT;

#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("DONE!\n");
#endif // DEBUG_MODE
}

static void packet_payload_extract_digits(float value, uint8_t *digits)
{
    int8_t digit_2 = -1;
    while(value >= 0)
    {
        value -= 1.00;
        digit_2++;
    }
    value += 1.00;
    
    int8_t digit_1 = -1;
    while(value >= 0)
    {
        value -= 0.10;
        digit_1++;
    }
    value += 0.10;
    
    int8_t digit_0 = -1;
    while(value >= 0)
    {
        value -= 0.01;
        digit_0++;
    }
    
    digits[0] = (uint8_t)digit_2;
    digits[1] = (uint8_t)digit_1;
    digits[2] = (uint8_t)digit_0;
}

//! \} End of packets group
