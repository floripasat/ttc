/*
 * timers.h
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
 * \file timers.h
 * 
 * \brief Initialization and control of the timers.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/03/2017
 * 
 * \defgroup timers Timers
 * \ingroup modules
 * \{
 */

#ifndef TIMERS_H_
#define TIMERS_H_

/**
 * \fn timer_init()
 * 
 * \brief This function initializes the TIMER A in continuous mode.
 * 
 * Start timer A in continuous mode sourced by SMCLK with a period of one second
 * and using compare mode.
 * 
 * \return None
 */
void timer_init();

#endif // TIMERS_H_

//! \} End of timers group
