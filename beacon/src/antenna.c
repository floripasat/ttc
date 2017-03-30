/*
 * antenna.c
 * 
 * Copyright (C) 2016, Universidade Federal de Santa Catarina
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
 * \file antenna.c
 * 
 * \brief Initialization and control of the antenna deployment pin.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 17/10/2016
 * 
 * \addtogroup antenna
 * \{
 */

#include "../inc/antenna.h"
#include "../inc/debug.h"
#include "../driverlib/driverlib.h"

uint8_t antenna_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Antenna initialization... ");
#endif // DEBUG_MODE

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE

    return STATUS_SUCCESS;
}

uint8_t antenna_IsReleased()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Checking if the antenna is released... ");
#endif // DEBUG_MODE

#if DEBUG_MODE == true
    debug_PrintMsg("RELEASED, nothing to do!\n");
#endif // DEBUG_MODE

    return ANTENNA_STATUS_RELEASED;
}

void antenna_Release()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Deploying the antenna... ");
#endif // DEBUG_MODE
    
#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

//! \} End of antenna group
