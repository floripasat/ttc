/*
 * antenna.h
 * 
 * Copyright (C) 2019, Universidade Federal de Santa Catarina.
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
 * \brief Antenna module configuration.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.18
 * 
 * \date 23/02/2019
 * 
 * \defgroup antenna_config Configuration
 * \ingroup antenna
 * \{
 */

#ifndef ANTENNA_CONFIG_H_
#define ANTENNA_CONFIG_H_

#define ANTENNA_MODULE_NAME                         "Antenna"

#define ANTENNA_MEM_ADR_DEPLOY_STATUS_0             0x10
#define ANTENNA_MEM_ADR_DEPLOY_STATUS_1             0x20
#define ANTENNA_MEM_ADR_DEPLOY_STATUS_2             0x40

#define ANTENNA_ARMING_TIMEOUT_S                    5
#define ANTENNA_ARMING_TIMEOUT_MS                   (ANTENNA_ARMING_TIMEOUT_S*1000)
#define ANTENNA_ARMING_ATTEMPTS                     5

#define ANTENNA_INDEPENDENT_DEPLOYMENT_BURN_TIME_S  10
#define ANTENNA_INDEPENDENT_DEPLOYMENT_BURN_TIME_MS (ANTENNA_INDEPENDENT_DEPLOYMENT_BURN_TIME_S*1000)

#define ANTENNA_SEQUENTIAL_DEPLOYMENT_BURN_TIME_S   (4*ANTENNA_OVERRIDE_DEPLOYMENT_BURN_TIME_S)
#define ANTENNA_SEQUENTIAL_DEPLOYMENT_BURN_TIME_MS  (ANTENNA_SEQUENTIAL_DEPLOYMENT_BURN_TIME_S*1000)

#endif // ANTENNA_CONFIG_H_

//! \} End of antenna_config group
