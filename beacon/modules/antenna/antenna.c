/*
 * antenna.c
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
 * \file antenna.c
 * 
 * \brief Antenna module implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 15/06/2017
 * 
 * \addtogroup antenna
 * \{
 */

#include <config/config.h>
#include <drivers/drivers.h>
#include <modules/debug/debug.h>

#include "antenna.h"

uint8_t antenna_init()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Antenna initialization... ");
#endif // DEBUG_MODE

#if BEACON_ANTENNA == ISIS_ANTENNA
    if (antenna_Init() = STATUS_SUCCESS)
    {
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("SUCCESS!\n");
    #endif // DEBUG_MODE
        return STATUS_SUCCESS;
    }
    else
    {
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("FAIL!\n");
    #endif // DEBUG_MODE
        return STATUS_FAIL;
    }
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("SUCCESS!\n");
    #endif // DEBUG_MODE
    
    return STATUS_SUCCESS;
#endif // BEACON_ANTENNA
}

uint8_t antenna_is_released()
{
    
}

uint8_t antenna_deploy()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Checking if the antenna is released... ");
#endif // DEBUG_MODE

    if (antenna_IsReleased() == ANTENNA_STATUS_RELEASED)
    {
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("RELEASED!\n");
    #endif // DEBUG_MODE
        return STATUS_SUCCESS;
    }
    else
    {
    #if BEACON_MODE == DEBUG_MODE
        debug_print_msg("NOT RELEASED!\n");
        debug_print_msg("Deploying the antenna... ");
    #endif // DEBUG_MODE
        if (antenna_Release() == STATUS_SUCCESS)
        {
        #if BEACON_MODE == DEBUG_MODE
            debug_print_msg("SUCCESS!\n");
        #endif // DEBUG_MODE
            return STATUS_SUCCESS;
        }
        else
        {
        #if BEACON_MODE == DEBUG_MODE
            debug_print_msg("FAIL!\n");
        #endif // DEBUG_MODE
            return STATUS_FAIL;
        }
    }
}

//! \} End of antenna group