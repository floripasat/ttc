/*
 * ax25.h
 * 
 * Copyright (C) 2016, Federal University of Santa Catarina.
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
 * \brief Functions of the AX25 protocol.
 * 
 * This module have all the functions to generate/manipulate packets with the AX25 protocol.
 *
 * \see AX.25 Link Access Protocol for Amateur Packet Radio. Version 2.2 Revision: July 1998.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 16/12/2016
 * 
 * \defgroup ax25 AX25
 * \ingroup src
 * \{
 */

#ifndef AX25_H_
#define AX25_H_

#include <stdint.h>

#include "ax25_config.h"
#include "ax25_fields.h"

/**
 * \brief The struct with the fields of the transfer frame header of the AX25.
 */
typedef struct
{
    uint8_t callsign[7];
    uint8_t ssid;
} AX25_Transfer_Frame_Header;

/**
 * \brief The struct with the packet payload data and length (in bytes).
 */
typedef struct
{
    uint8_t data[256];
    uint8_t len;
} AX25_Pkt_Payload;

/**
 * \brief The struct with all the AX25 fields.
 */
typedef struct
{
    uint8_t start_flag;
    AX25_Transfer_Frame_Header dst_adr;
    AX25_Transfer_Frame_Header src_adr;
    uint8_t control_bits;
    uint8_t protocol_id;
    AX25_Pkt_Payload payload;
    uint16_t fcs;
    uint8_t end_flag;
} AX25_Packet;

/**
 * \brief A struct to hold a bit variable.
 * 
 * This struct is used during the bit stuffing process.
 */
typedef struct
{
    uint8_t val:1;      /**< The bit value (0 or 1). */
} bit;

/**
 * \brief Generates the packet with a initial data.
 * 
 * The header of the Beacon AX25 packet is defined with following configuration:
 *      - AX25_FLAG
 *      - AX25_FLORIPASAT_CALLSIGN << 1
 *      - AX25_ADR_NON_REPEATER_SSID | (AX25_FLORIPASAT_GRS_SSID << 1) | AX25_ADR_DST_COMMAND
 *      - AX25_FLORIPASAT_CALLSIGN << 1
 *      - AX25_ADR_NON_REPEATER_SSID | (AX25_FLORIPASAT_SAT_SSID << 1) | AX25_ADR_SRC_COMMAND
 *      - AX25_CTRL_UNNUMBERED_UI | AX25_CTRL_PF_DISABLE
 *      - AX25_PID_NO_LAYER_3
 *      - Data
 *      - FCS (crc16 over the data field)
 *      - AX25_FLAG
 *      .
 * 
 * \param ax25_packet is the struct with an entire packet.
 * \param data is data to be transmitted at the data field of the packet.
 * \param data_size is size of the data to be transmitted at the data field of the packet.
 * 
 * \return None.
 */
void ax25_beacon_pkt_gen(AX25_Packet *ax25_packet, uint8_t *data, uint16_t data_size);

/**
 * \brief Updates the data field of an already generated packet.
 * 
 * \param ax25_packet is the packet who the data will be modify.
 * \param new_data is the new data of the data field of the packet.
 * \param new_data_size is size of the new data of the data field of the packet.
 * 
 * \return None.
 */
void ax25_update_data_from_pkt(AX25_Packet *ax25_packet, uint8_t *new_data, uint16_t new_data_size);

/**
 * \brief Converts a packet in a struct, to an array (string) of bytes.
 * 
 * \param ax25_packet is the packet to be converted.
 * \param str_pkt is the array containing the converted packet.
 * \param str_pkt_len is the size of the data field of the packet.
 * 
 * \return None.
 */
void ax25_pkt_2_str(AX25_Packet *ax25_packet, uint8_t *str_pkt, uint16_t *str_pkt_len);

/**
 * \brief Applies bit stuffing to an AX25 packet.
 * 
 * In order to ensure that the flag bit sequence mentioned above does not appear
 * accidentally anywhere else in a frame, the sending station monitors the bit sequence
 * for a group of five or more contiguous "1" bits. Any time five contiguous "1" bits are
 * sent, the sending station inserts a "0" bit after the fifth "1" bit. During frame reception,
 * any time five contiguous "1" bits are received, a "0" bit immediately following five "1"
 * bits is discarded.
 * 
 * \param pkt is the packet to apply the bit stuffing.
 * \param pkt_len is the length of the packet (in bytes).
 * \param new_pkt is an array to store the new packet (with the bit suttfing applied).
 * \param new_pkt_len is the length if the new_pkt (in bytes).
 * 
 * \return None.
 */
void ax25_bit_stuffing(uint8_t *pkt, uint16_t pkt_len, uint8_t *new_pkt, uint16_t *new_pkt_len);

/**
 * \brief Encodes a pre-generated AX25 packet to a ready-to-transmit format.
 * 
 * \param ax25_pkt is a AX25_Packet struct containing a pre-generated AX25 packet.
 * \param pkt is the resulting AX25 packet ready to transmit (in an array of bytes format).
 * \param pkt_len is the length of the pkt (in bytes).
 * 
 * \return None.
 */
void ax25_encode(AX25_Packet *ax25_pkt, uint8_t *pkt, uint16_t *pkt_len);

#endif // AX25_H_

//! \} End of AX25 group
