/*
 * antenna.c
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Antenna module implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.6
 * 
 * \date 15/06/2017
 * 
 * \addtogroup antenna
 * \{
 */

#include <config/config.h>
#include <system/debug/debug.h>

#if BEACON_ANTENNA == ISIS_ANTENNA
    #include <drivers/antenna/isis_antenna.h>
#endif // ISIS_ANTENNA

#include "antenna.h"

bool antenna_init()
{
    debug_print_msg("Antenna initialization... ");

#if BEACON_ANTENNA == ISIS_ANTENNA
    
    isis_antenna_init();

    debug_print_msg("SUCCESS!\n\r");

    return true;
    
#elif BEACON_ANTENNA == PASSIVE_ANTENNA

    debug_print_msg("SUCCESS!\n\r");
    
    return  true;
    
#endif // BEACON_ANTENNA
}

bool antenna_is_released()
{
#if BEACON_ANTENNA == ISIS_ANTENNA

    return isis_antenna_is_released();

#elif BEACON_ANTENNA == PASSIVE_ANTENNA

    return true;

#endif // BEACON_ANTENNA
}

bool antenna_deploy()
{
#if BEACON_ANTENNA == ISIS_ANTENNA
    
    isis_antenna_release();
    
    return isis_antenna_is_released();
    
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
    
    return true;

#endif // BEACON_ANTENNA
}

//! \} End of antenna group
