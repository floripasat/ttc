/*
 * time.h
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
 * \brief Time control configuration.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.9
 * 
 * \date 23/02/2019
 * 
 * \defgroup time_config Configuration
 * \ingroup time
 * \{
 */

#ifndef TIME_CONFIG_H_
#define TIME_CONFIG_H_

#include <config/memory.h>
#include <hal/mcu/flash.h>

#define TIME_MODULE_NAME            "System Time"

#define TIME_CRC8_INITIAL_VALUE     0x00        /**< CRC8-CCITT. */
#define TIME_CRC8_POLYNOMIAL        0x07        /**< CRC8-CCITT. */

#define TIME_SAVE_PERIOD_S          60          /**< Period (in seconds) to save the current time count value. */

// Memory
#define TIME_MEMORY_REGION          MEMORY_REGION_SYSTEM_TIME
#define TIME_VALUE_ADDRESS          MEMORY_ADR_TIME_COUNT
#define TIME_CHECKSUM_ADDRESS       MEMORY_ADR_TIME_COUNT_CHECKSUM
#define TIME_VALUE_BKP_ADDRESS      MEMORY_ADR_TIME_COUNT_BKP
#define TIME_CHECKSUM_BKP_ADDRESS   MEMORY_ADR_TIME_COUNT_BKP_CHECKSUM

#endif // TIME_CONFIG_H_

//! \} End of time_config group
