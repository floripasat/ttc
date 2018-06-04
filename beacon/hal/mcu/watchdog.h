/*
 * watchdog.h
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
 * \file watchdog.h
 * 
 * \brief Watchdog module header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup watchdog Watchdog
 * \ingroup hal
 * \{
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/**
 * \brief Initialization of the watchdog.
 * 
 * This function starts the watchdog with a 3 seconds counter in watchdog mode.
 * 
 * \return None.
 */
void watchdog_init();

/**
 * \brief Resets the watchdog timer.
 * 
 * \return None.
 */
void watchdog_reset_timer();

/**
 * \brief Disables the watchdog timer.
 * 
 * \return None.
 */
void watchdog_hold();

#endif // WATCHDOG_H_

//! \} End of watchdog group
