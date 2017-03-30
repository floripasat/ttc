/*
 * debug.c
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

#include "../inc/pkt_payload.h"
#include "../inc/debug.h"

uint8_t pkt_payload_GetSize(uint8_t *eps_data)
{
    return (sizeof(SAT_ID)-1 + sizeof("-")-1 + sizeof(BATT1_ID)-1 + sizeof(BATT2_ID)-1 + sizeof("=")-1 + sizeof("=")-1 + sizeof(eps_data)-1);
}

void pkt_payload_gen(uint8_t *pkt_payload, uint8_t *eps_data)
{
#if DEBUG_MODE == true
    debug_PrintMsg("Generating packet payload... ");
#endif // DEBUG_MODE

    uint8_t i = 0;
    uint8_t pkt_payload_counter = 0;
    for(i=0;i<sizeof(SAT_ID)-1;i++)
        pkt_payload[pkt_payload_counter++] = SAT_ID[i];
    
    pkt_payload[pkt_payload_counter++] = "-"[0];
    
    for(i=0;i<sizeof(BATT1_ID)-1;i++)
        pkt_payload[pkt_payload_counter++] = BATT1_ID[i];
    
    pkt_payload[pkt_payload_counter++] = "="[0];
    pkt_payload[pkt_payload_counter++] = eps_data[0];
    pkt_payload[pkt_payload_counter++] = eps_data[1];
    pkt_payload[pkt_payload_counter++] = "-"[0];
    
    for(i=0;i<sizeof(BATT2_ID)-1;i++)
        pkt_payload[pkt_payload_counter++] = BATT2_ID[i];
    
    pkt_payload[pkt_payload_counter++] = "="[0];
    pkt_payload[pkt_payload_counter++] = eps_data[2];
    pkt_payload[pkt_payload_counter++] = eps_data[3];

#if DEBUG_MODE == true
    debug_PrintMsg("DONE!\n");
#endif // DEBUG_MODE
}

void pkt_payload_update(uint8_t *pkt_payload, uint8_t *eps_data)
{
#if DEBUG_MODE == true
    debug_PrintMsg("Updating data of an already generated packet payload... ");
#endif // DEBUG_MODE

    uint8_t pkt_payload_counter = sizeof(SAT_ID)-1 + sizeof("-")-1 + sizeof(BATT1_ID)-1 + sizeof("=")-1;
    pkt_payload[pkt_payload_counter++] = eps_data[0];
    pkt_payload[pkt_payload_counter++] = eps_data[1];
    
    uint8_t i = 0;
    for(i=0;i<=(sizeof(BATT2_ID)-1+sizeof("=")-1);i++)
        pkt_payload_counter++;
    
    pkt_payload[pkt_payload_counter++] = eps_data[2];
    pkt_payload[pkt_payload_counter++] = eps_data[3];

#if DEBUG_MODE == true
    debug_PrintMsg("DONE!\n");
#endif // DEBUG_MODE
}

//! \} End of pkt_payload implementation group
