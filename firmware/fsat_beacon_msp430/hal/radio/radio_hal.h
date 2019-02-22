/*
 * radio_hal.h
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
 * \file radio_hal.h
 * 
 * \brief Radio HAL header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \defgroup radio_hal Radio
 * \ingroup hal
 * \{
 */

#ifndef RADIO_HAL_H_
#define RADIO_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#include <system/queue/queue.h>

/**
 * \brief Queue to store the RX data from the radio device.
 */
extern Queue radio_rx_queue;

/**
 * \brief Initialization of the radio module.
 * 
 * \return Initialization status.
 */
bool radio_init();

/**
 * \brief Resets the radio module.
 * 
 * \return None.
 */
void radio_reset();

/**
 * \brief Writes data to the radio module.
 * 
 * \param data is a pointer to an array of bytes to be written in the radio.
 * \param len is the lenght of the data to be written.
 * 
 * \return None.
 */
void radio_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads data from the radio module.
 * 
 * \param len is the length of the RX FIFO data.
 * 
 * \return None.
 */
void radio_read(uint8_t len);

/**
 * \brief Put the radio to sleep.
 * 
 * \return None.
 */
void radio_sleep();

/**
 * \brief Wakes up de radio module.
 * 
 * \return None.
 */
void radio_wake_up();

/**
 * \brief Enables the RX on the radio module.
 * 
 * \return None.
 */
void radio_enable_rx();

/**
 * \brief Disables the RX of the radio (If available, enables the standby mode).
 * 
 * \return None.
 */
void radio_disable_rx();

/**
 * \brief Returns the number of bytes available in the radio RX queue.
 * 
 * \return The number of bytes received from the radio and not processed yet.
 */
uint8_t radio_available();

/**
 * \brief Pops a byte from the radio RX queue.
 * 
 * \return The first available byte from the radio RX queue.
 */
uint8_t radio_pop();

/**
 * \brief Configures the ISR on a pin of the radio module (If available).
 * 
 * \return None.
 */
static void radio_init_rx_isr();

/**
 * \brief Pushes data from the radio RX (When available) to the radio RX queue.
 * 
 * \param byte is the byte to be pushed into the radio RX queue.
 * 
 * \return None.
 */
static void radio_push(uint8_t byte);

#endif // RADIO_HAL_H_

//! \} End of radio_hal group
