/*
 * flash.h
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
 * \file flash.h
 * 
 * \brief Flash memory control functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/06/2017
 * 
 * \defgroup flash Flash Memory
 * \ingroup modules
 * \{
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

/**
 * \fn flash_init
 * 
 * \brief 
 * 
 * \return The initialization status. It can be:
 *              -\b STATUS_SUCCESS
 *              -\b STATUS_FAIL
 *              .
 */
uint8_t flash_init();

/**
 * \fn flash_read_byte
 * 
 * \brief Reads a byte from a address from the flash memory.
 * 
 * \param adr is the memory address to read the byte.
 * 
 * \return The readed byte.
 */
uint8_t flash_read_byte(uint16_t adr);

/**
 * \fn flash_write_byte
 * 
 * \brief Writes a byte to a memory address in the flash memory.
 * 
 * \param byte is the byte to be written in the flash memory.
 * \param adr is memory address to write the byte.
 * 
 * \return None
 */
void flash_write_byte(uint8_t byte, uint16_t adr);

/**
 * \fn flash_read_data
 * 
 * \brief Reads data from the flash memory.
 * 
 * \param data is a pointer to array to write the data that will be read.
 * \param size is the size of the data that will be read.
 * \param adr is memory address of the start of the data that will be read.
 * 
 * \return None
 */
void flash_read_data(uint8_t *data, uint8_t size, uint8_t adr);

/**
 * \fn flash_write_data
 * 
 * \brief Writes data to the flash memory.
 * 
 * \param data is a pointer to an array of data to be written in the flash memory.
 * \param size is size of the data that will be written in flash memory.
 * \param adr is address of the first byte that will be written in the flash memory.
 * 
 * \return None
 */
void flash_write_data(uint8_t *data, uint8_t size, uint8_t adr);

#endif // FLASH_H_

//! \} End of flash group
