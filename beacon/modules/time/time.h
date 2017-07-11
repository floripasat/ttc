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

#define MILLISECONDS    0x00
#define SECONDS         0x01
#define MINUTES         0x02
#define HOURS           0x03
#define DAYS            0x04
#define WEEKS           0x05
#define MONTHS          0x06
#define YEARS           0x07

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

#endif // TIME_H_

//! \} End of time group
