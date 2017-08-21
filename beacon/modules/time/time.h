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
 * \brief Time control variables.
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

#define MILLISECONDS                0
#define SECONDS                     1
#define MINUTES                     2
#define HOURS                       3
#define DAYS                        4
#define WEEKS                       5
#define MONTHS                      6
#define YEARS                       7

#define TIME_REF_GREATER_THAN_CMP   1
#define TIME_CMP_GREATER_THAN_REF   2
#define TIME_CMP_EQUAL_REF          3

/**
 * \struct Time
 * 
 * \brief Time units struct.
 */
typedef struct
{
    uint16_t millisecond;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t week;
    uint8_t month;
    uint8_t year;
} Time;

/**
 * \fn time_reset
 * 
 * \brief Resets all the time counters.
 * 
 * \param time is a pointer to a Time object.
 * 
 * \return None
 */
void time_reset(Time *time);

/**
 * \fn time_compare
 * 
 * \brief Compares two Times structures (Compares if a time measure is greater, lesser or equal than other).
 * 
 * \note For now, it only compares day, hour, minute and second.
 * 
 * \param time_ref is a pointer to the reference time structure.
 * \param time_cmp is a pointer to the time structure to compare.
 * 
 * \return It returns:
 *          -\b TIME_REF_GREATER_THAN_CMP
 *          -\b TIME_CMP_GREATER_THAN_REF
 *          -\b TIME_CMP_EQUAL_REF
 *          .
 */
uint8_t time_compare(Time *time_ref, Time *time_cmp);

/**
 * \fn time_diff
 * 
 * \brief Calculates the time difference between two time structures.
 * 
 * \param time_a is the first time structure.
 * \param time_b is the second time structure.
 * \param result is the result (The difference between time_a and time_b).
 * 
 * \return None
 */
void time_diff(Time time_a, Time time_b, Time *result);

/**
 * \fn time_copy
 * 
 * \brief Copies a Time structure to another one.
 * 
 * \param time_a is a pointer to a Time structure to be copied.
 * \param time_b is a pointer to a Time structure to receive the copy.
 * 
 * \return None
 */
void time_copy(Time *time_a, Time *time_b);

#endif // TIME_H_

//! \} End of time group
