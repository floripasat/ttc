/*
 * radio_hal.h
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
 * \brief Radio HAL definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.15
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

/**
 * \brief Radio modes.
 */
typedef enum
{
    RADIO_MODE_STANDBY = 0,     /**< Standby mode. */
    RADIO_MODE_TX,              /**< TX mode. */
    RADIO_MODE_RX               /**< RX mode. */
} radio_modes_e;

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
 * \brief Reads data from the radio module buffer.
 * 
 * \param[in,out] data is an array to store the received data.
 * \param[in] len is the number of bytes to read from the radio buffer.
 * 
 * \return None.
 */
void radio_read(uint8_t *data, uint8_t len);

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
 * \brief Verifies if a new data was received or not.
 * 
 * \return TRUE/FALSE if there is available data to receive or not.
 */
bool radio_available();

#endif // RADIO_HAL_H_

//! \} End of radio_hal group
