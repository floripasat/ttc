/*
 * packet_payload.h
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
 * \file packets_payload.h
 * 
 * \brief Packet payload struct.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 11/07/2017
 * 
 * \defgroup pkt_pl Packet  Payload
 * \ingroup modules
 * \{
 */

#ifndef PACKET_PAYLOAD_H_
#define PACKET_PAYLOAD_H_

#include <stdint.h>

#include <config/config.h>
#include <modules/modules.h>

#define PKT_PAYLOAD_SAT_ID  SATELLITE_ID

#if OBDH_COM_DATA_PKT_LEN >= EPS_COM_DATA_PKT_LEN
    #define PKT_PAYLOAD_LEN (sizeof(SATELLITE_ID)-1 + OBDH_COM_DATA_PKT_LEN)
#else
    #define PKT_PAYLOAD_LEN (sizeof(SATELLITE_ID)-1 + EPS_COM_DATA_PKT_LEN)
#endif // PKT_PAYLOAD_LEN

/**
 * \struct PacketPayload
 * 
 * \brief PacketPayload struct.
 * 
 * This struct contains the payload (a byte array) and the length of the payload in bytes.
 */
typedef struct
{
    uint8_t payload[PKT_PAYLOAD_LEN + 1];
    uint8_t length;
} PacketPayload;

#endif // PACKET_PAYLOAD_H_

//! \} End of pkt_pl group
