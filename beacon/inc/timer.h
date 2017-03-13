/*
 * timer.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \file timer.h
 * 
 * \brief Initialization and control of timer A.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/03/2017
 * 
 * \defgroup timer Timer
 * \ingroup timer
 * \{
 */

#include "../driverlib/driverlib.h"

/**
 * \fn timer_Init()
 * 
 * \brief This function initializes the TIMER A in continuous mode.
 * 
 * Start timer A in continuous mode sourced by SMCLK with a period of one second
 * and using compare mode.
 * 
 * \return None
 */
void timer_Init();

//! \} End of timer group
