/*
 * time.h
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
 * \file time.h
 * 
 * \brief Time control module.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/06/2017
 * 
 * \defgroup time Time
 * \ingroup modules
 * \{
 */

#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

/**
 * \var second
 * 
 * \brief Time control variable (seconds since boot).
 */
extern uint32_t *second;

/**
 * \fn time_init
 * 
 * \brief 
 * 
 * \param second_ptr is a pointer to the second counter variable.
 * 
 * \return None
 */
void time_init(uint32_t *second_ptr);

/**
 * \fn time_timer_init
 * 
 * \brief This function initializes the TIMER A in continuous mode.
 * 
 * Start timer A in continuous mode sourced by SMCLK with a period of one second
 * and using compare mode.
 * 
 * This timer is used to control the time.
 * 
 * \return None
 */
static void time_timer_init();

/**
 * \fn time_timer_start
 * 
 * \brief Starts the time control timer operation.
 * 
 * \return None
 */
void time_timer_start();

/**
 * \fn time_reset
 * 
 * \brief Resets all the time counters.
 * 
 * \return None
 */
void time_reset();

#endif // TIME_H_

//! \} End of time group
