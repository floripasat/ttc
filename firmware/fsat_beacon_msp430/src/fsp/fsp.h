/*
 * fsp.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-FSP.
 * 
 * FloripaSat-FSP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-FSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with FloripaSat-FSP. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief FloripaSat Protocol library implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.4
 * 
 * \date 06/11/2017
 * 
 * \defgroup fsp FSP
 * \ingroup src
 * \{
 */

#ifndef FSP_H_
#define FSP_H_

#include <stdint.h>

// Packet positions
#define FSP_PKT_POS_SOD                 0       /**< Star-of-data byte position. */
#define FSP_PKT_POS_SRC_ADR             1       /**< Source address byte position. */
#define FSP_PKT_POS_DST_ADR             2       /**< Destination address byte position. */
#define FSP_PKT_POS_LEN                 3       /**< Length byte position. */
#define FSP_PKT_POS_TYPE                4       /**< Type byte position. */

// Start Of Data byte
#define FSP_PKT_SOD                     0x7E    /**< Star-of-data byte. */

// Addresses
#define FSP_ADR_EPS                     1       /**< EPS module address. */
#define FSP_ADR_TTC                     2       /**< TTC module address. */
#define FSP_ADR_OBDH                    3       /**< OBDH module address. */

// Types of packets
#define FSP_PKT_TYPE_DATA               1       /**< Data packet. */
#define FSP_PKT_TYPE_DATA_WITH_ACK      2       /**< Data with ack packet. */
#define FSP_PKT_TYPE_CMD                3       /**< Command packet. */
#define FSP_PKT_TYPE_CMD_WITH_ACK       4       /**< Command with ack packet. */
#define FSP_PKT_TYPE_ACK                5       /**< Acknowledge packet. */
#define FSP_PKT_TYPE_NACK               6       /**< Not-acknowledge packet. */

// Commands
#define FSP_CMD_NOP                     1       /**< No-operation command. */
#define FSP_CMD_SEND_DATA               2       /**< Send data command. */
#define FSP_CMD_REQUEST_RF_MUTEX        3       /**< Request RF mutex command. */
#define FSP_CMD_HIBERNATION             4       /**< Hibernation command. */

// Ack answers
#define FSP_ACK_RF_MUTEX_FREE           1       /**< RF mutex free acknowledge answer. */
#define FSP_ACK_RF_MUTEX_BUSY           2       /**< RF mutes busy acknowledge answer. */

// Max. lengths
#define FSP_PKT_MAX_LENGTH              256     /**< Packet maximum length (in bytes). */
#define FSP_PAYLOAD_MAX_LENGTH          252     /**< Payload maximum length (in bytes). */

// CRC16 initial value (or seed byte)
#define FSP_CRC16_INITIAL_VALUE         0       /**< CRC16 initial value. */

// Decode states
#define FSP_PKT_NOT_READY               0       /**< Packet not ready state. */
#define FSP_PKT_READY                   1       /**< Packet ready state. */
#define FSP_PKT_INVALID                 2       /**< Packet invalid state. */
#define FSP_PKT_WRONG_ADR               3       /**< Packet with wrong address state. */
#define FSP_PKT_ERROR                   4       /**< Packet with error state. */

// Config.
#define FSP_PKT_WITH_ACK                1       /**< Packet with ack. */
#define FSP_PKT_WITHOUT_ACK             0       /**< Packet without acknowledge. */

/**
 * \brief FSP packet struct.
 */
typedef struct
{
    uint8_t sod;                                /**< Start of data. */
    uint8_t src_adr;                            /**< Source address. */
    uint8_t dst_adr;                            /**< Destination address. */
    uint8_t length;                             /**< Length of the packet payload. */
    uint8_t type;                               /**< Type of packet. */
    uint8_t payload[FSP_PAYLOAD_MAX_LENGTH];    /**< Payload of the packet. */
    uint16_t crc16;                             /**< CRC16-CCITT bytes. */
} FSPPacket;

/**
 * \brief The address of the module running this library.
 */
extern uint8_t fsp_my_adr;

/**
 * \brief Decode byte position (From the internal "state machine").
 */
extern uint8_t fsp_decode_pos;

/**
 * \brief Initializes the FSP library.
 * 
 * \param module_adr is the address of the module running this library.
 * 
 * \return None.
 */
void fsp_init(uint8_t module_adr);

/**
 * \brief Resets the FSP internal state machine (The position counter) for decoding.
 * 
 * \return None.
 */
void fsp_reset();

/**
 * \brief Generates a FSP data packet.
 * 
 * \param data is the paylod of the desired data packet.
 * \param data_len is the length of the payload of the desired data packet.
 * \param dst_adr is the destination address.
 * \param ack is the acknowledge option (FSP_PKT_WITH_ACK or FSP_PKT_WITHOUT_ACK).
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None.
 */
void fsp_gen_data_pkt(uint8_t *data, uint8_t data_len, uint8_t dst_adr, uint8_t ack, FSPPacket *fsp);

/**
 * \brief Generates a FSP command packet.
 * 
 * \param cmd is the command of the desired command packet.
 * \param dst_adr is the destination address.
 * \param ack is the acknowledge option (FSP_PKT_WITH_ACK or FSP_PKT_WITHOUT_ACK).
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None.
 */
void fsp_gen_cmd_pkt(uint8_t cmd, uint8_t dst_adr, uint8_t ack, FSPPacket *fsp);

/**
 * \brief Generates a FSP ack. packet.
 * 
 * The ack. packet does not have a payload (The length field is zero).
 * 
 * \param dst_adr is the destination address.
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None.
 */
void fsp_gen_ack_pkt(uint8_t dst_adr, FSPPacket *fsp);

/**
 * \brief Generates a FSP nack. packet.
 * 
 * The nack. packet does not have a payload (The length field is zero).
 * 
 * \param dst_adr is the destination address.
 * \param fsp is a pointer to a FSPPacket struct to store the new data packet.
 * 
 * \return None.
 */
void fsp_gen_nack_pkt(uint8_t dst_adr, FSPPacket *fsp);

/**
 * \brief Generates a generic FSP packet from a given data.
 * 
 * \param payload is the payload of the desired packet.
 * \param payload_len is the length of the payload of the desired packet.
 * \param dst_adr is the destination address.
 * \param type is the type of the packet.
 * 
 * \return None.
 */
void fsp_gen_pkt(uint8_t *payload, uint8_t payload_len, uint8_t dst_adr, uint8_t type, FSPPacket *fsp);

/**
 * \brief Encodes a FSPPacket struct into a array of bytes.
 * 
 * \param fsp is a pointer to a FSPPacket struct with all the packet data.
 * \param pkt is an array to store the packet.
 * \param pkt_len is a pointer to the packet array length.
 * 
 * \return None.
 */
void fsp_encode(FSPPacket *fsp, uint8_t *pkt, uint8_t *pkt_len);

/**
 * \brief Decodes a array with the FSP packet into a FSPPacket struct.
 * 
 * \param byte is the incoming byte from a FSP packet.
 * \param fsp is a pointer to a FSPPacket struct to store the packet data.
 * 
 * \return The state of the decoding process. It can be:
 *              - FSP_PKT_NOT_READY when all the packet data were not received yet.
 *              - FSP_PKT_READY when the packet is ready to be used and the decoding process is over.
 *              - FSP_PKT_INVALID when an invalid packet was received.
 *              - FSP_PKT_WRONG_ADR when the destination address of the packet is not equal to host address.
 *              - FSP_PKT_ERROR when an error occurs during the decoding process.
 *              .
 */
uint8_t fsp_decode(uint8_t byte, FSPPacket *fsp);

#endif // FSP_H_

//! \} End of fsp group
