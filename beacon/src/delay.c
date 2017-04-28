/*
 * debug.c
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 *
 */ 

/**
 * \file delay.c
 * 
 * \brief Debug functions implementation
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \addtogroup delay
 * \{
 */

#include <msp430.h>

#include "../inc/delay.h"

void delay_s(uint8_t s)
{
    while(s--)
    {
        __delay_cycles(DELAY_FREQUENCY_CPU);
    }
}

void delay_ms(uint16_t ms)
{
    while(ms--)
    {
        __delay_cycles(DELAY_FREQUENCY_CPU/1000);
    }
}

void delay_us(uint32_t us)
{
    while(us--)
    {
        __delay_cycles(DELAY_FREQUENCY_CPU/1000000);
    }
}

//! \} End of delay implementation group
