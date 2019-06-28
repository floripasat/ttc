/*
 * beacon_config.h
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
 * FloripaSat-TTc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Beacon configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.5
 * 
 * \date 07/03/2019
 * 
 * \defgroup beacon_config Configuration
 * \ingroup beacon
 * \{
 */

#ifndef BEACON_CONFIG_H_
#define BEACON_CONFIG_H_

#include <config/memory.h>
#include <hal/mcu/flash.h>

#define BEACON_MODULE_NAME                                  "Beacon"

#define BEACON_SAVE_PARAMS_PERIOD_S                         60

// Memory
#define BEACON_PARAMS_MEMORY_REGION                         MEMORY_REGION_SYSTEM_PARAMS
#define BEACON_PARAM_HIBERNATION_MEM_ADR                    MEMORY_ADR_PARAM_HIBERNATION
#define BEACON_PARAM_ENERGY_LEVEL_MEM_ADR                   MEMORY_ADR_PARAM_ENERGY_LEVEL
#define BEACON_PARAM_LAST_ENERGY_LEVEL_SET_MEM_ADR          MEMORY_ADR_PARAM_LAST_ENERGY_LEVEL_SET
#define BEACON_PARAM_LAST_TIME_VALID_PKT_MEM_ADR            MEMORY_ADR_PARAM_LAST_TIME_VALID_PKT
#define BEACON_PARAM_EPS_LAST_TIME_VALID_PKT_MEM_ADR        MEMORY_ADR_PARAM_EPS_LAST_TIME_VALID_PKT
#define BEACON_PARAM_EPS_ERRORS_MEM_ADR                     MEMORY_ADR_PARAM_EPS_ERRORS
#define BEACON_PARAM_EPS_IS_DEAD_PKT_MEM_ADR                MEMORY_ADR_PARAM_EPS_IS_DEAD_PKT
#define BEACON_PARAM_OBDH_LAST_TIME_VALID_PKT_MEM_ADR       MEMORY_ADR_PARAM_OBDH_LAST_TIME_VALID_PKT
#define BEACON_PARAM_OBDH_ERRORS_MEM_ADR                    MEMORY_ADR_PARAM_OBDH_ERRORS
#define BEACON_PARAM_OBDH_IS_DEAD_PKT_MEM_ADR               MEMORY_ADR_PARAM_OBDH_IS_DEAD_PKT
#define BEACON_PARAM_PARAMS_SAVED_MEM_ADR                   MEMORY_ADR_PARAM_PARAMS_SAVED
#define BEACON_PARAM_PARAMS_DEPLOU_HIB_EXECUTED_MEM_ADR     MEMORY_ADR_PARAM_PARAMS_DEPLOU_HIB_EXECUTED
#define BEACON_PARAM_DEPLOYMENT_ATTEMPTS_MEM_ADR            MEMORY_ADR_PARAM_DEPLOYMENT_ATTEMPTS
#define BEACON_PARAM_HIBERNATION_MODE_INITIAL_TIME_MEM_ADR  MEMORY_ADR_HIBERNATION_MODE_INITIAL_TIME
#define BEACON_PARAM_HIBERNATION_DURATION_MEM_ADR           MEMORY_ADR_HIBERNATION_MODE_DURATION

#endif // BEACON_CONFIG_H_

//! \} End of beacon_config group
