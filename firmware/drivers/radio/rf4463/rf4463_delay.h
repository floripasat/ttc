/*
 * rf4463_delay.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina.
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
 * \brief RF4463 delay functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \addtogroup rf4463
 * \{
 */

#ifndef RF4463_DELAY_H_
#define RF4463_DELAY_H_

#include <stdint.h>

/**
 * \brief Seconds delay.
 * 
 * \param s is the delay in seconds.
 * 
 * \return None.
 */
void rf4463_delay_s(uint8_t s);

/**
 * \brief Milliseconds delay.
 * 
 * \param ms is the delay in milliseconds.
 * 
 * \return None.
 */
void rf4463_delay_ms(uint16_t ms);

/**
 * \brief Microseconds delay
 * 
 * \param us is the delay in microseconds.
 * 
 * \return None.
 */
void rf4463_delay_us(uint32_t us);

#endif // RF4463_DELAY_H_

//! \} End of rf4463 group
