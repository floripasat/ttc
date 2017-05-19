/*
 * pkt_payload.c
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \file pkt_payload.c
 * 
 * \brief Packet payload functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 02/02/2017
 * 
 * \addtogroup pkt_payload
 * \{
 */

#include <stdbool.h>

#include "../inc/pkt_payload.h"
#include "../inc/debug.h"

void pkt_payload_Gen(uint8_t *pkt_payload, uint8_t *eps_data)
{
#if DEBUG_MODE == true
    debug_PrintMsg("Generating packet payload... ");
#endif // DEBUG_MODE

    uint8_t i = 0;
    uint8_t pkt_payload_counter = 0;
    for(i=0; i<sizeof(PKT_PAYLOAD_SAT_ID)-1; i++)
    {
        pkt_payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_ID[i];
    }
    
    pkt_payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_BATT_SEPARATOR[0];
    
    for(i=0; i<sizeof(PKT_PAYLOAD_BATT1_ID)-1; i++)
    {
        pkt_payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT1_ID[i];
    }
    
    pkt_payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT_VALUE_SEPARATOR[0];
    
    uint8_t digits[4];
    pkt_payload_ExtractDigits(0.004883*((eps_data[0] << 8) + eps_data[1]), digits);
    
    pkt_payload[pkt_payload_counter++] = digits[0];
    pkt_payload[pkt_payload_counter++] = 0x2C;      // 0x2C = ','
    pkt_payload[pkt_payload_counter++] = digits[1];
    pkt_payload[pkt_payload_counter++] = digits[2];
    pkt_payload[pkt_payload_counter++] = 0x56;      // 0x56 = 'V'
    pkt_payload[pkt_payload_counter++] = PKT_PAYLOAD_SAT_BATT_SEPARATOR[0];
    
    for(i=0; i<sizeof(PKT_PAYLOAD_BATT2_ID)-1; i++)
    {
        pkt_payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT2_ID[i];
    }
    
    pkt_payload[pkt_payload_counter++] = PKT_PAYLOAD_BATT_VALUE_SEPARATOR[0];
    
    pkt_payload_ExtractDigits(0.004883*((eps_data[2] << 8) + eps_data[3]), digits);
    
    pkt_payload[pkt_payload_counter++] = digits[0];
    pkt_payload[pkt_payload_counter++] = 0x2C;      // 0x2C = ','
    pkt_payload[pkt_payload_counter++] = digits[1];
    pkt_payload[pkt_payload_counter++] = digits[2];
    pkt_payload[pkt_payload_counter++] = 0x56;      // 0x56 = 'V'

#if DEBUG_MODE == true
    debug_PrintMsg("DONE!\n");
#endif // DEBUG_MODE
}

void pkt_payload_Update(uint8_t *pkt_payload, uint8_t *eps_data)
{
#if DEBUG_MODE == true
    debug_PrintMsg("Updating data of an already generated packet payload... ");
#endif // DEBUG_MODE

    uint8_t pkt_payload_counter = sizeof(PKT_PAYLOAD_SAT_ID)-1 + sizeof(PKT_PAYLOAD_SAT_BATT_SEPARATOR)-1 + sizeof(PKT_PAYLOAD_BATT1_ID)-1 + sizeof(PKT_PAYLOAD_BATT_VALUE_SEPARATOR)-1;
    
    uint8_t digits[4];
    pkt_payload_ExtractDigits(0.004883*((eps_data[0] << 8) + eps_data[1]), digits);
    
    pkt_payload[pkt_payload_counter++] = digits[0];
    pkt_payload[pkt_payload_counter++] = 0x2C;      // 0x2C = ','
    pkt_payload[pkt_payload_counter++] = digits[1];
    pkt_payload[pkt_payload_counter++] = digits[2];
    pkt_payload[pkt_payload_counter++] = 0x56;      // 0x56 = 'V'
    
    uint8_t i = 0;
    for(i=0; i<=(sizeof(PKT_PAYLOAD_BATT2_ID)-1 + sizeof(PKT_PAYLOAD_BATT_VALUE_SEPARATOR)-1); i++)
    {
        pkt_payload_counter++;
    }
    
    pkt_payload_ExtractDigits(0.004883*((eps_data[2] << 8) + eps_data[3]), digits);
    
    pkt_payload[pkt_payload_counter++] = digits[0];
    pkt_payload[pkt_payload_counter++] = 0x2C;      // 0x2C = ','
    pkt_payload[pkt_payload_counter++] = digits[1];
    pkt_payload[pkt_payload_counter++] = digits[2];
    pkt_payload[pkt_payload_counter++] = 0x56;      // 0x56 = 'V'

#if DEBUG_MODE == true
    debug_PrintMsg("DONE!\n");
#endif // DEBUG_MODE
}

static void pkt_payload_ExtractDigits(float value, uint8_t *digits)
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

//! \} End of pkt_payload implementation group
