/*
 * antenna_delay.c
 * 
 * Copyright (C) 2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-TTC.
 * 
 * FloripaSat-TTC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-TTC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Antenna delay implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.7
 * 
 * \date 13/03/2019
 * 
 * \defgroup antenna_delay Delay
 * \ingroup antenna
 * \{
 */

#include <system/system.h>

#include "antenna.h"

void antenna_delay_s(uint32_t s)
{
    uint8_t i = 0;
    for(i=0; i<s; i++)
    {
        system_enter_low_power_mode();

        watchdog_reset_timer();
    }
}

//! \} End of antenna_delay group
