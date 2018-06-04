/*
 * eps_hal.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina
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
 * \file eps_hal.h
 * 
 * \brief EPS HAL functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup eps_hal EPS
 * \ingroup hal
 * \{
 */

#ifndef EPS_HAL_H_
#define EPS_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#include <system/queue/queue.h>

/**
 * \brief EPS queue to store all the incoming data.
 */
extern Queue eps_queue;

/**
 * \brief Initialization of the EPS communication module.
 * 
 * \param eps is a pointer to an EPS object.
 * 
 * \return Initialization status. True/False if the initialization was successful or not.
 */
bool eps_init();

/**
 * \brief Returns the number of bytes available in the EPS queue.
 * 
 * \return The number of bytes available in the EPS queue.
 */
uint8_t eps_available();

/**
 * \brief Pops a byte from the EPS queue.
 * 
 * \return The first byte available in the EPS queue.
 */
uint8_t eps_pop();

/**
 * \brief Reads n bytes of data from the EPS queue.
 * 
 * \param data is a pointer to an array to put the data from the queue.
 * \param bytes is the number of bytes to read from the queue.
 * 
 * \return None.
 */
void eps_read(uint8_t *data, uint8_t bytes);

/**
 * \brief Clears the data buffer.
 * 
 * \param eps is a pointer to an EPS object.
 * 
 * \return None.
 */
void eps_clear();

/**
 * \brief EPS HAL UART initialization.
 * 
 * \return Initialization status. It can be:
 *              - TRUE
 *              - FALSE
 *              .
 */
static bool eps_hal_uart_init();

/**
 * \brief Pushes data into the EPS queue.
 * 
 * \param byte is the byte to be pushed into the EPS queue.
 * 
 * \return None.
 */
static void eps_push(uint8_t byte);

#endif // EPS_HAL_H_

//! \} End of eps_hal group
