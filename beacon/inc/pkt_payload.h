/*
 * debug.h
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
 * \file pkt_payload.h
 * 
 * \brief Functions for create/manipulate the payload of a packet.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 02/03/2017
 * 
 * \defgroup pkt_payload Packet Payload
 * \ingroup beacon
 * \{
 */

#ifndef PKT_PAYLOAD_H_
#define PKT_PAYLOAD_H_

#include <stdint.h>

#define SAT_ID      "FLORIPASAT"
#define BATT1_ID    "BATT1"
#define BATT2_ID    "BATT2"

/**
 * \fn pkt_payload_GetSize
 * 
 * \brief This function calculates the total length of the packet payload.
 * 
 * \param eps_data is a pointer to the packet payload.
 * 
 * \return The length of the packet payload.
 */
uint8_t pkt_payload_GetSize(uint8_t *eps_data);

/**
 * \fn pkt_payload_gen
 * 
 * \brief This function creates the payload of a packet, with the IDs and
 * the telemetry data.
 * 
 * \param pkt_payload is a pointer to the packet payload.
 * \param eps_data is a pointer to the telemetry data to sent.
 * 
 * \return None
 */
void pkt_payload_gen(uint8_t *pkt_payload, uint8_t *eps_data);

/**
 * \fn pkt_payload_update
 * 
 * \brief This function updates only the telemetry data of a already
 * created packet.
 * 
 * \param pkt_payload is a pointer to the packet payload.
 * \param eps_data is a pointer to the telemetry data to be sent.
 * 
 * \return None
 */
void pkt_payload_update(uint8_t *pkt_payload, uint8_t *eps_data);

#endif // PKT_PAYLOAD_H_

//! \} End of pkt_payload group
