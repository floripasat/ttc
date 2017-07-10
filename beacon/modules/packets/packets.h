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
 * \brief Packets handling routines.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 14/06/2017
 * 
 * \defgroup packets Packets
 * \ingroup modules
 * \{
 */

#ifndef PACKETS_H_
#define PACKETS_H_

#include <stdint.h>
#include <config/config.h>
#include <modules/eps_com/eps_com.h>

#define PKT_PAYLOAD_SAT_ID                  SATELLITE_ID
#define PKT_PAYLOAD_SAT_BATT_SEPARATOR      "-"
#define PKT_PAYLOAD_BATT1_ID                "BATT1"
#define PKT_PAYLOAD_BATT2_ID                "BATT2"
#define PKT_PAYLOAD_BATT_VALUE_SEPARATOR    "="
#define PKT_PAYLOAD_DECIMAL_SEPARATOR       ','
#define PKT_PAYLOAD_VOLTAGE_UNIT            'V'

#define PKT_PAYLOAD_LEN                     (sizeof(PKT_PAYLOAD_SAT_ID)-1 + sizeof(PKT_PAYLOAD_SAT_BATT_SEPARATOR)-1 + sizeof(PKT_PAYLOAD_BATT1_ID)-1 + sizeof(PKT_PAYLOAD_BATT2_ID)-1 + 2*5 + 2*sizeof(PKT_PAYLOAD_BATT_VALUE_SEPARATOR)-1)

/**
 * \struct PacketPayload
 * 
 * \brief PacketPayload struct.
 * 
 * This struct contains the payload (a byte array) and the lenght of the payload in bytes.
 */
typedef struct
{
    uint8_t payload[PKT_PAYLOAD_LEN + 1];
    uint8_t lenght;
} PacketPayload;

/**
 * \var packet_payload
 * 
 * \brief packet_payload object.
 */
extern PacketPayload packet_payload;

/**
 * \fn packet_payload_generate
 * 
 * \brief Generates a packet payload with the given data.
 * 
 * \param pkt_payload is the packet payload object.
 * 
 * \return None
 */
void packet_payload_generate(PacketPayload pkt_payload, EPSData eps_data);

/**
 * \fn packet_payload_extract_digits
 * 
 * \brief Extract the digist from a given float variable.
 * 
 * \param value is the float number to extract the digits.
 * \param digits is a pointer to an array of bytes with the result of the extraction.
 * 
 * \return None
 */
static void packet_payload_extract_digits(float value, uint8_t *digits);

#endif // PACKETS_H_

//! \} End of packets group
