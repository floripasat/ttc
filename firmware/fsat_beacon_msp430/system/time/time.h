/*
 * time.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * \brief Time control module.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.9
 * 
 * \date 10/06/2017
 * 
 * \defgroup time Time
 * \ingroup system
 * \{
 */

#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

#define TIME_MIN_TO_SEC(x)      (x*60)      /**< Minutes to seconds conversion. */
#define TIME_SEC_TO_MIN(x)      (x/60)      /**< Seconds to minutes conversion. */

/**
 * \brief Time struct.
 */
typedef struct
{
    uint32_t second_counter;    /**< Seconds counter. */
    uint8_t crc8;               /**< Checksum of the seconds counter using the CRC8 algorithm. */
} Time;

/**
 * \brief Time control variable.
 */
extern Time time;

/**
 * \brief Backup of the main time control variable (Time control redundance).
 */
extern Time time_backup;

/**
 * \brief System time initialization.
 * 
 * The system time initialization load the initial value of the time and
 * initializes the time control timer.
 * 
 * \return None.
 */
void time_init();

/**
 * \brief This function initializes a timer in continuous mode.
 * 
 * Start timer in continuous mode sourced by TIME_TIMER_CLOCK_SOURCE with a
 * period of one second and using compare mode.
 * 
 * This timer is used to control the time.
 * 
 * \return None.
 */
static void time_timer_init();

/**
 * \brief CRC8 checksum.
 * 
 * \param time_counter is the time value to get the CRC8 value.
 * 
 * This function converts a 32-bit time counter to an 4 element 8-bit array
 * to get the CRC8 value of the time counter.
 * 
 * \return The crc8 value of the time counter variable.
 */
static uint8_t time_crc8(uint32_t time_counter);

/**
 * \brief Saves the current system time to the non-volation memory.
 *
 * \return None.
 */
static void time_save();

/**
 * \brief Loads the system time from the non-volatile memory.
 *
 * \return None.
 */
static void time_load();

/**
 * \brief Starts the time control timer operation (timer continuous mode).
 * 
 * \return None.
 */
void time_timer_start();

/**
 * \brief Resets all the time counter.
 * 
 * \return None.
 */
void time_reset();

/**
 * \brief Returns the system time, in seconds.
 * 
 * \return The seconds counter value.
 */
uint32_t time_get_seconds();

/**
 * \brief Returns the system time, in minutes.
 *
 * \return The seconds counter value in minutes.
 */
uint32_t time_get_minutes();

#endif // TIME_H_

//! \} End of time group
