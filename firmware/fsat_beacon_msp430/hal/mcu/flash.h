/*
 * flash.h
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
 * \brief Flash memory control functions definitions.
 * 
 * \author Matheus dos Santos Frata
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.9
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

#define FLASH_FULL                  1
#define FLASH_NOT_FULL              0

#define FLASH_BANK_0                0
#define FLASH_BANK_1                1
#define FLASH_BANK_2                2
#define FLASH_BANK_3                3

// 128 kB banks
#define FLASH_BANK_0_ADR            0x00008000
#define FLASH_BANK_1_ADR            0x00028000
#define FLASH_BANK_2_ADR            0x00048000
#define FLASH_BANK_3_ADR            0x00068000

// 128 B info segments
#define FLASH_SEG_A_ADR             0x00001980
#define FLASH_SEG_B_ADR             0x00001900
#define FLASH_SEG_C_ADR             0x00001880
#define FLASH_SEG_D_ADR             0x00001800

// 512 B bootstrap segments
#define FLASH_BSL_0_ADR             0x00001600
#define FLASH_BSL_1_ADR             0x00001400
#define FLASH_BSL_2_ADR             0x00001200
#define FLASH_BSL_3_ADR             0x00001000
#define FLASH_MASS_ERASE            0X00FFFFFF

// First boot start adress
#define FLASH_BOOT_ADDR             FLASH_BANK_1_ADR

//Flash pointer save adress
#define FLASH_PTR_ADDR              FLASH_SEG_C_ADR

// Overflow flag message adress
#define FLASH_OVERFLOW_FLAG_ADDR    0x00026000

// Last adress that can write a data(beyond this will enter the overflow)
#define FLASH_LAST_WRITE_ADDR       0x00087FFF

/**
 * \brief Writes data into the flash memory.
 *
 * \param[in,out] data is a pointer to the data to be written.
 * \param[in] len is the number of bytes to be written.
 *
 * \return None.
 */
void flash_write(uint8_t *data, uint16_t len);

/**
 * \brief Writes a single byte.
 *
 * \param[in] data is the byte to be written.
 * \param[in,out] addr is the address to write the given byte.
 *
 * \return None.
 */
void flash_write_single(uint8_t data, uint8_t *addr);

/**
 * \brief Writes a 32-bits integer.
 *
 * \param[in] data is the 32-bit integer to be written.
 * \param[in,out] addr is the address to write the given integer.
 *
 * \return None.
 */
void flash_write_long(uint32_t data, uint32_t *addr);

/**
 * \brief Erases a memory region.
 *
 * \param[in,out] region is the memory region to erase.
 *
 * \return None.
 */
void flash_erase(uint32_t *region);

/**
 * \brief Reads data from a memory address.
 *
 * \param[in,out] addr is the address to read.
 *
 * \return None.
 */
uint8_t flash_read_single(uint8_t *addr);

/**
 * \brief Reads data from a memory address.
 *
 * \param[in,out] addr is the address to read.
 *
 * \return None.
 */
uint32_t flash_read_long(uint32_t *addr);

#endif // FLASH_H_

//! \} End of flash group
