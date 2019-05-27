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
 * \version 0.4.5
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
#define MEMORY_REGION_SYSTEM_TIME                   FLASH_SEG_A_ADR
#define MEMORY_ADR_TIME_COUNT                       (uint32_t *)(FLASH_SEG_A_ADR)
#define MEMORY_ADR_TIME_COUNT_CHECKSUM              (uint8_t *)(FLASH_SEG_A_ADR + 8)
#define MEMORY_ADR_TIME_COUNT_BKP                   (uint32_t *)(FLASH_SEG_A_ADR + 16)
#define MEMORY_ADR_TIME_COUNT_BKP_CHECKSUM          (uint8_t *)(FLASH_SEG_A_ADR + 24)
#define MEMORY_ADR_HIBERNATION_MODE_INITIAL_TIME    (uint32_t *)(FLASH_SEG_A_ADR + 32)
#define MEMORY_ADR_HIBERNATION_MODE_DURATION        (uint32_t *)(FLASH_SEG_A_ADR + 40)

// System parameters
#define MEMORY_REGION_SYSTEM_PARAMS                 FLASH_SEG_D_ADR
#define MEMORY_ADR_PARAM_HIBERNATION                (uint8_t *)(FLASH_SEG_D_ADR)
#define MEMORY_ADR_PARAM_ENERGY_LEVEL               (uint8_t *)(FLASH_SEG_D_ADR + 4)
#define MEMORY_ADR_PARAM_LAST_ENERGY_LEVEL_SET      (uint8_t *)(FLASH_SEG_D_ADR + 8)
#define MEMORY_ADR_PARAM_LAST_TIME_VALID_PKT        (uint8_t *)(FLASH_SEG_D_ADR + 12)
#define MEMORY_ADR_PARAM_EPS_LAST_TIME_VALID_PKT    (uint8_t *)(FLASH_SEG_D_ADR + 16)
#define MEMORY_ADR_PARAM_EPS_ERRORS                 (uint8_t *)(FLASH_SEG_D_ADR + 20)
#define MEMORY_ADR_PARAM_EPS_IS_DEAD_PKT            (uint8_t *)(FLASH_SEG_D_ADR + 24)
#define MEMORY_ADR_PARAM_OBDH_LAST_TIME_VALID_PKT   (uint8_t *)(FLASH_SEG_D_ADR + 28)
#define MEMORY_ADR_PARAM_OBDH_ERRORS                (uint8_t *)(FLASH_SEG_D_ADR + 32)
#define MEMORY_ADR_PARAM_OBDH_IS_DEAD_PKT           (uint8_t *)(FLASH_SEG_D_ADR + 36)
#define MEMORY_ADR_PARAM_PARAMS_SAVED               (uint8_t *)(FLASH_SEG_D_ADR + 40)
#define MEMORY_ADR_PARAM_PARAMS_DEPLOU_HIB_EXECUTED (uint8_t *)(FLASH_SEG_D_ADR + 44)
#define MEMORY_ADR_PARAM_DEPLOYMENT_ATTEMPTS        (uint8_t *)(FLASH_SEG_D_ADR + 48)

#endif // MEMORY_H_

//! \} End of memory group
