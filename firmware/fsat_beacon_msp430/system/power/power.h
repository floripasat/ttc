/*
 * power.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \brief System power control.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 24/11/2017
 * 
 * \defgroup power
 * \ingroup system
 * \{
 */

#ifndef POWER_H_
#define POWER_H_

/**
 * \brief Makes the MCU enter in low-power mode.
 * 
 * \return None.
 */
void system_enter_low_power_mode();

/**
 * \brief Resets the system using a software BOR.
 * 
 * \return None.
 */
void system_reset();

/**
 * \brief Shutdowns the system.
 * 
 * \return None.
 */
void system_shutdown();

#endif // POWER_H_

//! \} End of power group
