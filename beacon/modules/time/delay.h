/*
 * delay.h
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
 * \file delay.h
 * 
 * \brief Delay functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup delay Delay
 * \ingroup modules
 * \{
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

/**
 * \fn delay_s
 * 
 * \brief Seconds delay.
 * 
 * \param s is the delay in seconds.
 * 
 * \return None
 */
void delay_s(uint8_t s);

/**
 * \fn delay_ms
 * 
 * \brief Milliseconds delay.
 * 
 * \param ms is the delay in milliseconds.
 * 
 * \return None
 */
void delay_ms(uint16_t ms);

/**
 * \fn delay_us
 * 
 * \brief Microseconds delay
 * 
 * \param us is the delay in microseconds.
 * 
 * \return None
 */
void delay_us(uint32_t us);

#endif // DELAY_H_

//! \} End of delay group
