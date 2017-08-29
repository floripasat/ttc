/*
 * radio.h
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
 * \file radio.h
 * 
 * \brief Radio module header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \defgroup radio Radio
 * \ingroup modules
 * \{
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>

/**
 * \fn radio_init
 * 
 * \brief Initialization of the radio module.
 * 
 * \return 
 */
uint8_t radio_init();

/**
 * \fn radio_reset
 * 
 * \brief Resets the radio module.
 * 
 * \return None
 */
void radio_reset();

/**
 * \fn radio_write_data
 * 
 * \brief Writes data to the radio module.
 * 
 * \param data is a pointer to an array of bytes to be written in the radio.
 * \param len is the lenght of the data to be written.
 * 
 * \return None
 */
void radio_write_data(uint8_t *data, uint16_t len);

/**
 * \fn radio_read_data
 * 
 * \brief Read data from the radio module.
 * 
 * \param buffer is a pointer to an array to store the RX FIFO data.
 * \param len is the length of the RX FIFO data.
 * 
 * \return None
 */
void radio_read_data(uint8_t *buffer, uint8_t *len);

/**
 * \fn radio_sleep
 * 
 * \brief Put the radio to sleep.
 * 
 * \return None
 */
void radio_sleep();

/**
 * \fn radio_wake_up
 * 
 * \brief Wakes up de radio module.
 * 
 * \return None
 */
void radio_wake_up();

/**
 * \fn radio_init_rx_isr
 * 
 * \brief Configures the ISR on a pin of the radio module (If available).
 * 
 * \return None
 */
void radio_init_rx_isr();

/**
 * \fn radio_enable_rx
 * 
 * \brief Enables the RX on the radio module.
 * 
 * \return None
 */
void radio_enable_rx();

#endif // RADIO_H_

//! \} End of radio group
