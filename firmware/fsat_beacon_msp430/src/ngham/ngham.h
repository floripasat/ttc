/*
 * ngham.h
 * 
 * Copyright (C) 2014, Jon Petter Skagmo
 * Copyright (C) 2017, Gabriel Mariano Marcelino
 * 
 * This file is part of FloripaSat-TTC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>
 * 
 */

/**
 * \brief NGHam protocol functions.
 * 
 * \author Jon Petter Skagmo <web@skagmo.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.11
 *
 * \date 10/02/2017
 * 
 * \defgroup ngham NGHam
 * \ingroup src
 * \{
 */

#ifndef NGHAM_H_
#define NGHAM_H_

#include <stdint.h>

#include "fec.h"
#include "ngham_packets.h"

#define NGH_PREAMBLE_SIZE               4
#define NGH_SYNC_SIZE                   4
#define NGH_PREAMBLE_SIZE_FOUR_LEVEL    8
#define NGH_SYNC_SIZE_FOUR_LEVEL        8
#define NGH_SIZE_TAG_SIZE               3
#define NGH_MAX_CODEWORD_SIZE           255
#define NGH_MAX_TOT_SIZE                (NGH_PREAMBLE_SIZE_FOUR_LEVEL+NGH_SYNC_SIZE_FOUR_LEVEL+NGH_SIZE_TAG_SIZE+NGH_MAX_CODEWORD_SIZE)

#define NGHAM_FOUR_LEVEL_MODULATION     0

#define NGH_PADDING_bm                  0x1F
#define NGH_FLAGS_bm                    0xE0
#define NGH_FLAGS_bp                    5

/**
 * \brief Packets number of sizes.
 * 
 * There are seven different sizes.
 * Each size has a correlation tag for size, a total size, a maximum payload size and a parity data size.
 */
#define NGH_SIZES                       7

/**
 * \brief Decoder states.
 */
#define NGH_STATE_SIZE_TAG              0
#define NGH_STATE_SIZE_TAG_2            1
#define NGH_STATE_SIZE_TAG_3            2
#define NGH_STATE_SIZE_KNOWN            3
#define NGH_STATE_STATUS                4
#define NGH_STATE_STATUS_2              5

/**
 * \brief Maximum number of errors in the size tag.
 */
#define NGH_SIZE_TAG_MAX_ERROR          6

#define NGH_HAMMING_DISTANCE_GREATER    0x00
#define NGH_HAMMING_DISTANCE_SMALLER    0x01

#define NGHAM_MODULE_NAME               "NGHam"

extern const uint8_t NGH_PL_SIZE[];             /**< Actual payload. */
extern const uint8_t NGH_PL_SIZE_FULL[];        /**< Size with LEN, payload and CRC. */
extern const uint8_t NGH_PL_PAR_SIZE[];         /**< Size with RS parity added. */
extern const uint8_t NGH_PAR_SIZE[];            /**< . */

extern const uint8_t NGH_SYNC[];

extern const uint32_t NGH_SIZE_TAG[];

extern const uint8_t NGH_PREAMBLE;              /**< . */
extern const uint8_t NGH_SYNC[];                /**< . */
extern const uint8_t NGH_PREAMBLE_FOUR_LEVEL;   /**< . */
extern const uint8_t NGH_SYNC_FOUR_LEVEL[];     /**< . */

extern RS rs_cb[NGH_SIZES];                     /**< Reed Solomon control blocks for the different NGHAM sizes. */

/**
 * \brief NGHam initialization.
 * 
 * \return None
 */
void ngham_init();

/**
 * \brief Generates Reed Solomon tables.
 * 
 * Run only once - generates reed solomon tables for all 7 packet sizes
 * MM=8, genpoly=0x187, fcs=112, prim=11, nroots=32 or 16
 * 
 * \return None
 */
void ngham_init_arrays();

/**
 * \brief Free Reed Solomon tables.
 * 
 * \return None
 */
void ngham_deinit_arrays();

/**
 * \brief Packet encoding.
 * 
 * Packets to be transmitted are passed to this function - max. length 220 B
 * 
 * \param *p
 * \param *pkt
 * \param *pkt_len
 * 
 * \return None
 */
void ngham_encode(NGHam_TX_Packet *p, uint8_t *pkt, uint16_t *pkt_len);

/**
 * \brief Packet decoding.
 * 
 * Packet to be decoded (Without preamble and sync. bytes).
 * 
 * \param d
 * \param *msg
 * \param *msg_len
 * 
 * \return The decodification state.
 */
uint8_t ngham_decode(uint8_t d, uint8_t *msg, uint8_t *msg_len);

#endif // NGHAM_H_

//! \} End of NGHam group
