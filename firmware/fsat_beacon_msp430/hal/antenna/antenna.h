/*
 * antenna.h
 * 
 * Copyright (C) 2017-2018, Universidade Federal de Santa Catarina.
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
 * \brief Antenna module header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.3
 * 
 * \date 15/06/2017
 * 
 * \defgroup antenna Antenna
 * \ingroup hal
 * \{
 */

#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <stdbool.h>

/**
 * \brief Antenna status.
 */
typedef enum
{
    ANTENNA_STATUS_NOT_DEPLOYED = 0,    /**< The antenna is not deployed. */
    ANTENNA_STATUS_DEPLOYED,            /**< The antenna is deployed. */
    ANTENNA_STATUS_UNKNOWN              /**< The antenna deployment status is unknown. */
} antenna_status_e;

/**
 * \brief Antenna module initialization.
 * 
 * \return TRUE/FALSE if successful or not.
 */
bool antenna_init();

/**
 * \brief Antenna deployment routine.
 * 
 * \return TRUE/FALSE if the deployment was successful or not.
 */
bool antenna_deploy();

/**
 * \brief Checks if the antenna is released or not.
 * 
 * \return The deployment status:
 * \parblock
 *      - ANTENNA_STATUS_NOT_DEPLOYED
 *      - ANTENNA_STATUS_DEPLOYED
 *      .
 * \endparblock
 */
uint8_t antenna_get_deployment_status();

/**
 * \brief Saves the deployment status in non-volatile memory.
 *
 * \param[in] status is the deployment status:
 * \parblock
 *      - ANTENNA_STATUS_NOT_DEPLOYED.
 *      - ANTENNA_STATUS_DEPLOYED.
 *      .
 * \endparblock
 *
 * \return None.
 */
void antenna_set_deployment_status(uint8_t status);

/**
 * \brief Delay period in seconds.
 *
 * \param[in] s is the delay period in seconds.
 *
 * \return None.
 */
void antenna_delay_s(uint32_t s);

#endif // ANTENNA_H_

//! \} End of antenna group
