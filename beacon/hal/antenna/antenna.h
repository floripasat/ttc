/*
 * antenna.h
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
 * \file antenna.h
 * 
 * \brief Antenna module header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 15/06/2017
 * 
 * \defgroup antenna Antenna module
 * \ingroup modules
 * \{
 */

#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <stdbool.h>

/**
 * \fn antenna_init
 * 
 * \brief Antenna module initialization.
 * 
 * \return Deployment status. It can be:
 *              -\b TRUE if the antenna initialization was successful.
 *              -\b FALSE if the antenna initialization was not successful.
 *              .
 */
bool antenna_init();

/**
 * \fn antenna_is_released
 * 
 * \brief Checks if the antenna is released or not.
 * 
 * \return Returns:
 *              -\b TRUE if the antenna is released.
 *              -\b FALSE if the antenna is not released.
 *              .
 */
bool antenna_is_released();

/**
 * \fn antenna_deploy
 * 
 * \brief Antenna deployment routine.
 * 
 * \return Deployment status. It can be:
 *              -\b TRUE if the deployment was successful.
 *              -\b FALSE if the deployment was not successful.
 *              .
 */
bool antenna_deploy();

#endif // ANTENNA_H_

//! \} End of antenna group
