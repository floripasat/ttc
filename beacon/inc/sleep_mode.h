/*
 * sleep_mode.h
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
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */
 
/**
 * \file sleep_mode.h
 * 
 * \brief Sleep mode control functions.
 * 
 * When the module enter in sleep mode, only the timer keep working. No transmissions
 * are made and radio is put in sleep mode too.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 24/03/2017
 * 
 * \defgroup sleep_mode Sleep Mode
 * \ingroup beacon
 * \{
 */

#ifndef SLEEP_MODE_H_
#define SLEEP_MODE_H_

#include <stdint.h>

#define BEACON_SLEEP_MODE_OFF           0x00
#define BEACON_SLEEP_MODE_ON            0x01
#define BEACON_SLEEP_MODE_PERIOD_HOUR   24

/**
 * \brief Sleep mode global variables.
 * 
 * \{
 */
extern uint8_t beacon_sleep_mode;                   /**< Sleep mode flag (TRUE = Sleep mode on, FALSE = Sleep mode off). */
extern uint8_t beacon_sleep_mode_initial_hour;      /**< Initial time when the sleep mode is activated. */
//! \}

/**
 * \fn sleep_mode_TurnOn
 * 
 * \brief Turn on the sleep mode.
 * 
 * This function enables the sleep mode, saves the initial time of the sleep
 * and mode and puts the radio in sleep mode too.
 * 
 * \return None
 */
void sleep_mode_TurnOn();

/**
 * \fn sleep_mode_TurnOff
 * 
 * \brief Turn off the sleep mode.
 * 
 * This function disables the sleep mode.
 * 
 * \return None
 */
void sleep_mode_TurnOff();

/**
 * \fn sleep_mode_getStatus
 * 
 * \brief This function returns the status of the sleep mode.
 * 
 * \return Returns the status of the sleep mode. It can be:
 *              -\b BEACON_SLEEP_MODE_ON
 *              -\b BEACON_SLEEP_MODE_OFF
 *              .
 */
uint8_t sleep_mode_getStatus();

#endif // SLEEP_MODE_H_

//! \} End of sleep_mode group
