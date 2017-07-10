/*
 * isis_antenna.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina
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
 * \file isis_antenna.h
 * 
 * \brief ISIS antenna driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/07/2017
 * 
 * \defgroup isis_antenna ISIS antenna
 * \ingroup drivers
 * \{
 */
 
#ifndef ISIS_ANTENNA_H_
#define ISIS_ANTENNA_H_

#define ANTENNA_STATUS_NOT_RELEASED     0x00
#define ANTENNA_STATUS_RELEASED         0x01

#include <stdint.h>

/**
 * \fn antenna_Init()
 * 
 * \brief Initialization of the antenna deployment peripherals.
 * 
 * \note TO BE DEVELOPED!
 * 
 * \return none
 */
uint8_t isis_antenna_Init();

/**
 * \fn antenna_IsReleased
 * 
 * \brief Verifies if the antenna is released or not.
 * 
 * \note TO BE DEVELOPED!
 * 
 * \return It can return:
 *              -\b ANTENNA_STATUS_NOT_RELEASED if the antenna is not released.
 *              -\b ANTENNA_STATUS_RELEASED if the antenna is released.
 *              .
 */
uint8_t isis_antenna_is_released();

/**
 * \fn antenna_Release
 * 
 * \brief Enables the antenna deployment.
 * 
 * \note TO BE DEVELOPED!
 * 
 * \return Deployment status. It can be:
 *              -\b STATUS_SUCCESS
 *              -\b STATUS_FAIL
 *              .
 */
uint8_t isis_antenna_release();

#endif // ANTENNA_H_

//! \} End of antenna group
