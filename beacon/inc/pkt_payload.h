/*
 * pkt_payload.h
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

#include "uart-eps.h"

/**
 * \brief Payload ID labels.
 * 
 * \{
 */
#define PKT_PAYLOAD_SAT_ID                  "FLORIPASAT"    /**< Satellite ID. */
#define PKT_PAYLOAD_SAT_BATT_SEPARATOR      "-"             /**< Satellite ID and batteries information separator. */
#define PKT_PAYLOAD_BATT1_ID                "BATT1"         /**< Battery 1 ID. */
#define PKT_PAYLOAD_BATT2_ID                "BATT2"         /**< Battery 2 ID. */
#define PKT_PAYLOAD_BATT_VALUE_SEPARATOR    "="             /**< Batteries IDs and batteries values separator. */
//! \}

/**
 * \brief Length of the packet payload.
 * 
 * \{
 */
#define PKT_PAYLOAD_LEN                     (sizeof(PKT_PAYLOAD_SAT_ID)-1 + sizeof(PKT_PAYLOAD_SAT_BATT_SEPARATOR)-1 + sizeof(PKT_PAYLOAD_BATT1_ID)-1 + sizeof(PKT_PAYLOAD_BATT2_ID)-1 + 2*5 + 2*sizeof(PKT_PAYLOAD_BATT_VALUE_SEPARATOR)-1)
//! \}

/**
 * \fn pkt_payload_Gen
 * 
 * \brief This function creates the payload of a packet, with the IDs and
 * the telemetry data.
 * 
 * \param pkt_payload is a pointer to the packet payload.
 * \param eps_data is a pointer to the telemetry data to sent.
 * 
 * \return None
 */
void pkt_payload_Gen(uint8_t *pkt_payload, uint8_t *eps_data);

/**
 * \fn pkt_payload_Update
 * 
 * \brief This function updates only the telemetry data of a already
 * created packet.
 * 
 * \param pkt_payload is a pointer to the packet payload.
 * \param eps_data is a pointer to the telemetry data to be sent.
 * 
 * \return None
 */
void pkt_payload_Update(uint8_t *pkt_payload, uint8_t *eps_data);

/**
 * \fn pkt_payload_ExtractDigits
 * 
 * \brief Extract the digits from a decimal number.
 * 
 * Example:
 * Number 3,45 -> digit2 = 3, digit1 = 4, digit0 = 5.
 * 
 * \param value is decimal number to extract the digits.
 * \param digits is the array to return the digits from the decimal number.
 * 
 * \return None
 */
static void pkt_payload_ExtractDigits(float value, uint8_t *digits);

#endif // PKT_PAYLOAD_H_

//! \} End of pkt_payload group
