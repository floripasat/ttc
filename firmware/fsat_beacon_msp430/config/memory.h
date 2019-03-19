/*
 * memory.h
 * 
 * Copyright (C) 2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-TTC.
 * 
 * FloripaSat-TTC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-TTC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Non-volatile memory configuration.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.9
 * 
 * \date 18/03/2019
 * 
 * \defgroup memory Memory
 * \ingroup config
 * \{
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <hal/mcu/flash.h>

// System time
#define MEMORY_REGION_SYSTEM_TIME               FLASH_SEG_A_ADR
#define MEMORY_ADR_TIME_COUNT                   (uint32_t *)(FLASH_SEG_A_ADR)
#define MEMORY_ADR_TIME_COUNT_CHECKSUM          (uint8_t *)(FLASH_SEG_A_ADR + 8)
#define MEMORY_ADR_TIME_COUNT_BKP               (uint32_t *)(FLASH_SEG_A_ADR + 16)
#define MEMORY_ADR_TIME_COUNT_BKP_CHECKSUM      (uint8_t *)(FLASH_SEG_A_ADR + 24)

#endif // MEMORY_H_

//! \} End of memory group
