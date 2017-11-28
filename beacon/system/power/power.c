/*
 * power.c
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
 * \file power.c
 * 
 * \brief System power control implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 24/11/2017
 * 
 * \addtogroup power
 * \{
 */

#include <msp430.h>
#include <config/config.h>

#include "power.h"

void system_enter_low_power_mode()
{
    _BIS_SR(LOW_POWER_MODE_ON);
}

void system_reset()
{
    //beacon_save_time();
    
    //WTDCTL = 0xDEAD;                // Reset system by writing to the WDT register without using the proper password
    
    PMMCTL0 = PMMPW | PMMSWBOR;     // Triggers a software BOR
}

void system_shutdown()
{
    //_BIS_SR(LPM4_5_bits);
}

//! \} End of power group
