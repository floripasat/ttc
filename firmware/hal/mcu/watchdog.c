/*
 * watchdog.c
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
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
 * \file watchdog.c
 * 
 * \brief Watchdog functions implementation
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \addtogroup watchdog
 * \{
 */

#include <config/config.h>
#include <drivers/driverlib/driverlib.h>

#include "watchdog.h"

void watchdog_init()
{
    // Watchdog initialization
    WDT_A_initWatchdogTimer(WATCHDOG_BASE_ADDRESS, WATCHDOG_CLK_SOURCE, WATCHDOG_CLK_DIVIDER);
    
    // Start counter
    WDT_A_start(WATCHDOG_BASE_ADDRESS);
}

void watchdog_reset_timer()
{
    WDT_A_resetTimer(WATCHDOG_BASE_ADDRESS);
}

void watchdog_hold()
{
    WDT_A_hold(WATCHDOG_BASE_ADDRESS);
}

//! \} End of watchdog group
