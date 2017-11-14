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
 * \ingroup hal
 * \{
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>
#include <stdbool.h>

// 128 kB banks
#define FLASH_BANK0_ADR     0x008000
#define FLASH_BANK1_ADR     0x028000
#define FLASH_BANK2_ADR     0x048000
#define FLASH_BANK3_ADR     0x068000

// 128 B info segments
#define FLASH_SEGD_ADDR     0x001800
#define FLASH_SEGC_ADDR     0x001880
#define FLASH_SEGB_ADDR     0x001900
#define FLASH_SEGA_ADDR     0x001980

// 512 B bootstrap segments
#define FLASH_BSL0_ADDR     0x001600
#define FLASH_BSL1_ADDR     0x001400
#define FLASH_BSL2_ADDR     0x001200
#define FLASH_BSL3_ADDR     0x001000
#define FLASH_MASS_ERASE    0XFFFFFF

/**
 * \var flash_ptr
 * 
 * \brief 
 */
extern uint8_t *flash_ptr;

/**
 * \fn flash_init
 * 
 * \brief 
 * 
 * \return The initialization status. It can be:
 *              -\b TRUE
 *              -\b FALSE
 *              .
 */
bool flash_init();

/**
 * \fn flash_read_byte
 * 
 * \brief Reads a byte from an address from the flash memory.
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

/**
 * \fn flash_read_word
 * 
 * \brief Reads a word (16-bits) from an address from the flash memory.
 * 
 * \param adr is the memory address to read the word.
 * 
 * \return The read word.
 */
uint16_t flash_read_word(uint16_t adr);

/**
 * \fn flash_write_word
 * 
 * \brief Writes a word (16-bits) to a memory address in the flash memory.
 * 
 * \param word is the word to be written in the flash memory.
 * \param adr is memory address to write the byte.
 * 
 * \return None
 */
void flash_write_word(uint16_t word, uint16_t adr);

/**
 * \fn flash_read_word32
 * 
 * \brief Reads a word (32-bits) from an address from the flash memory.
 * 
 * \param adr is the memory address to read the word.
 * 
 * \return The read word.
 */
uint32_t flash_read_word32(uint16_t adr);

/**
 * \fn flash_write_word32
 * 
 * \brief Writes a word (32-bits) to a memory address in the flash memory.
 * 
 * \param word32 is the word to be written in the flash memory.
 * \param adr is memory address to write the byte.
 * 
 * \return None
 */
void flash_write_word32(uint32_t word32, uint16_t adr);

#endif // FLASH_H_

//! \} End of flash group
