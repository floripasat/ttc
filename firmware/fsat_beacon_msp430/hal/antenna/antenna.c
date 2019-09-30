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
 * \version 0.5.19
 * 
 * \date 15/06/2017
 * 
 * \addtogroup antenna
 * \{
 */

#include <config/config.h>
#include <system/debug/debug.h>
#include <system/time/time.h>

#if BEACON_ANTENNA == ISIS_ANTENNA
    #include <drivers/antenna/isis_antenna.h>
#endif // ISIS_ANTENNA

#include "antenna.h"
#include "antenna_config.h"

void antenna_init()
{
    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Initialization...\n\r");

#if BEACON_ANTENNA == ISIS_ANTENNA
    isis_antenna_init();
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
#endif // BEACON_ANTENNA
}

void antenna_deploy()
{
    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Deploying the antenna...\n\r");

#if BEACON_ANTENNA == ISIS_ANTENNA
    // Arming the antenna module
    uint8_t i;
    for(i=0; i<ANTENNA_ARMING_ATTEMPTS; i++)
    {
        debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Trying to arm the antenna module (attempt ");
        debug_print_dec(i+1);
        debug_print_msg(" of ");
        debug_print_dec(ANTENNA_ARMING_ATTEMPTS);
        debug_print_msg(")...\n\r");

        if (isis_antenna_arm())
        {
            debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "The antenna module is armed!\n\r");

            break;
        }
        else
        {
            debug_print_event_from_module(DEBUG_ERROR, ANTENNA_MODULE_NAME, "Error arming the antenna module!\n\r");
        }

        antenna_delay_s(1);
    }

    if (i == ANTENNA_ARMING_ATTEMPTS)
    {
        debug_print_event_from_module(DEBUG_ERROR, ANTENNA_MODULE_NAME, "It was not possible to arm the antenna module! Trying to deploy without arm...\n\r");
    }

#if BEACON_ANTENNA_DEPLOY_MODE == ANTENNA_INDEPENDENT_DEPLOY_MODE
    // Executing independent deployment
    int ant;
    for(ant=ISIS_ANTENNA_ANT_1; ant<=ISIS_ANTENNA_ANT_4; ant++)
    {
        debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Deploying antenna ");
        debug_print_dec(ant);
        debug_print_msg(" independently...\n\r");

        isis_antenna_start_independent_deploy(ant, ANTENNA_INDEPENDENT_DEPLOYMENT_BURN_TIME_S, ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE);

        antenna_delay_s(ANTENNA_INDEPENDENT_DEPLOYMENT_BURN_TIME_S+2);
    }
#elif BEACON_ANTENNA_DEPLOY_MODE == ANTENNA_SEQUENTIAL_DEPLOY_MODE
    // Executing sequential deployment
    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Deploying antennas sequentially...\n\r");
    isis_antenna_start_sequential_deploy(ANTENNA_SEQUENTIAL_DEPLOYMENT_BURN_TIME_MS);
    antenna_delay_s(ANTENNA_SEQUENTIAL_DEPLOYMENT_BURN_TIME_S);
#endif // BEACON_ANTENNA_DEPLOY_MODE

    antenna_delay_s(5);     // Wait before disarming the antenna

    if (isis_antenna_disarm())
    {
        debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "The antenna module is disarmed!\n\r");
    }
    else
    {
        debug_print_event_from_module(DEBUG_ERROR, ANTENNA_MODULE_NAME, "Error disarming the antenna module!\n\r");
    }
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
#endif // BEACON_ANTENNA

    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Deployment procedure executed!\n\r");
}

uint8_t antenna_get_deployment_status()
{
#if BEACON_ANTENNA == ISIS_ANTENNA
    return (isis_antenna_get_antenna_status(ISIS_ANTENNA_ANT_1) &
            isis_antenna_get_antenna_status(ISIS_ANTENNA_ANT_2) &
            isis_antenna_get_antenna_status(ISIS_ANTENNA_ANT_3) &
            isis_antenna_get_antenna_status(ISIS_ANTENNA_ANT_4)) > 0 ? ANTENNA_STATUS_DEPLOYED : ANTENNA_STATUS_NOT_DEPLOYED;
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
    return ANTENNA_STATUS_DEPLOYED;
#endif // BEACON_ANTENNA
}

//! \} End of antenna group
