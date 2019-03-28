/*
 * obdh_hal.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief OBDH HAL functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.3
 * 
 * \date 23/03/2017
 * 
 * \defgroup obdh_hal OBDH
 * \ingroup hal
 * \{
 */

#ifndef OBDH_HAL_H_
#define OBDH_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#include "obdh_hal_config.h"

#include <system/queue/queue.h>

/**
 * \brief OBDH queue to store all the incoming data.
 */
extern Queue obdh_queue;

/**
 * \brief Initialization of the OBDH communication.
 * 
 * \return Initialization status. It can be:
 *              - TRUE
 *              - FALSE
 *              .
 */
bool obdh_init();

/**
 * \brief OBDH HAL SPI initialization.
 * 
 * \return Initialization status. It can be:
 *              - TRUE
 *              - FALSE
 *              .
 */
static bool obdh_hal_spi_init();

/**
 * \brief Enables the data reception from the OBDH module.
 *
 * \return None.
 */
void obdh_enable();

/**
 * \brief Disables the data reception from the OBDH module.
 *
 * \return None.
 */
void obdh_disable();

/**
 * \brief Returns the number of bytes available in the OBDH queue.
 * 
 * \return The number of bytes available in the OBDH queue.
 */
uint8_t obdh_available();

/**
 * \brief Pushes a byte to the OBDH queue.
 * 
 * \param byte is the new byte to push into the OBDH queue.
 * 
 * \return None.
 */
static void obdh_push(uint8_t byte);

/**
 * \brief Pops a byte from the OBDH queue.
 * 
 * \return The first byte available in the OBDH queue.
 */
uint8_t obdh_pop();

/**
 * \brief Reads n bytes of data from the OBDH queue.
 * 
 * \param data is a pointer to an array to put the data from the queue.
 * \param bytes is the number of bytes to read from the queue.
 * 
 * \return None.
 */
void obdh_read(uint8_t *data, uint8_t bytes);

/**
 * \brief Sends data to the OBDH module.
 * 
 * \param data is the byte to send to the OBDH module.
 * \param len is the length of the data (in bytes) to send.
 * 
 * \return None.
 */
void obdh_send(uint8_t *data, uint8_t len);

/**
 * \brief Clears the OBDH queue.
 * 
 * \return None.
 */
void obdh_clear();

#endif // OBDH_HAL_H_

//! \} End of obdh_hal group
