/*
 * rf4463_delay.c
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
 * \brief RF4463 delay functions implementation.
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

#include <msp430.h>
#include <config/config.h>

#include "rf4463_delay.h"

void rf4463_delay_s(uint8_t s)
{
    while(s--)
    {
        __delay_cycles(BEACON_CPU_FREQ_HZ);
    }
}

void rf4463_delay_ms(uint16_t ms)
{
    while(ms--)
    {
        __delay_cycles(BEACON_CPU_FREQ_HZ/1000);
    }
}

void rf4463_delay_us(uint32_t us)
{
    while(us--)
    {
        __delay_cycles(BEACON_CPU_FREQ_HZ/1000000);
    }
}

//! \} End of rf4463 group
