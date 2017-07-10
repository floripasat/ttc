/*
 * flash.c
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
 * \file flash.c
 * 
 * \brief Flash memory control functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/06/2017
 * 
 * \addtogroup flash
 * \{
 */

#include <libs/driverlib/driverlib.h>

#include "flash.h"

uint8_t flash_init()
{
    return 0x01;
}

uint8_t flash_read_byte(uint16_t adr)
{
    return 0xFF;
}

void flash_write_byte(uint8_t byte, uint16_t adr)
{
    
}

void flash_read_data(uint8_t *data, uint8_t size, uint8_t adr)
{
    
}

void flash_write_data(uint8_t *data, uint8_t size, uint8_t adr)
{
    
}

//! \} End of flash group
