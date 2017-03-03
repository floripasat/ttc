/*
 * antenna.h
 * 
 * Copyright (C) 2016, Universidade Federal de Santa Catarina
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file antenna.h
 * 
 * \brief Initialization and control of the antenna deployment pin.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 17/10/2016
 * 
 * \defgroup antenna Antenna
 * \ingroup beacon
 * \{
 */
 
#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <stdint.h>
#include "../driverlib/driverlib.h"

/**
 * \fn antenna_Init()
 * 
 * \brief Initialization of the antenna deployment pin.
 * 
 * \return none
 */
uint8_t antenna_Init();

/**
 * \fn antenna_IsReleased
 * 
 * \brief
 * 
 * \return 
 */
uint8_t antenna_IsReleased();

/**
 * \fn antenna_Release
 * 
 * \brief Enables the antenna deployment.
 * 
 * \return none
 */
void antenna_Release();

#endif // ANTENNA_H_

//! \} End of antenna group
