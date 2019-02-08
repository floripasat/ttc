/*
 * isis_antenna_i2c.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \brief ISIS antenna driver I2C interface.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 21/09/2017
 * 
 * \addtogroup isis_antenna
 * \{
 */

#ifndef ISIS_ANTENNA_I2C_H_
#define ISIS_ANTENNA_I2C_H_

#include <stdint.h>

/**
 * \brief I2C interface initialization.
 * 
 * \return None
 */
void isis_antenna_i2c_init();

/**
 * \brief Writes a byte in the I2C bus.
 * 
 * \param byte is the byte to write in the I2C bus.
 * 
 * \return None
 */
void isis_antenna_i2c_write_byte(uint8_t byte);

/**
 * \brief Writes data in the I2C bus.
 * 
 * \param data is an array of data to write in the I2C bus.
 * \param len is the length of the data.
 * 
 * \return None
 */
void isis_antenna_i2c_write_data(uint8_t *data, uint8_t len);

/**
 * \brief Read a byte from the I2C bus.
 * 
 * \return The byte from the I2C bus.
 */
uint8_t isis_antenna_i2c_read_byte();

/**
 * \brief Read n bytes from the I2C bus.
 * 
 * \param data is a pointe to write the data from the I2C bus.
 * \param len is the length of the data.
 * 
 * \return None.
 */
void isis_antenna_i2c_read_data(uint8_t *data, uint8_t len);

#endif // ISIS_ANTENNA_I2C_H_

//! \} End of isis_antenna group
