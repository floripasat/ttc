/*
 * rf_switch.c
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
 * \file rf_switch.c
 * 
 * \brief Control of the RF switch implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/03/2017
 * 
 * \addtogroup rf_switch
 * \{
 */

#include <config/config.h>

#if BEACON_RF_SWITCH == SKY13345_368LF
    #include <drivers/rf_switch/sky13345_368lf.h>
#endif // BEACON_RF_SWITCH

#include "rf_switch.h"

void rf_switch_init()
{
    
}

void rf_switch_enable_beacon()
{
    
}

void rf_switch_disable_beacon()
{
    
}

//! \} End of rf_switch group
