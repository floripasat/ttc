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
 * \version 0.2.3
 * 
 * \date 15/06/2017
 * 
 * \addtogroup antenna
 * \{
 */

#include <config/config.h>
#include <system/debug/debug.h>
#include <hal/mcu/flash.h>

#if BEACON_ANTENNA == ISIS_ANTENNA
    #include <drivers/antenna/isis_antenna.h>
#endif // ISIS_ANTENNA

#include "antenna.h"
#include "antenna_config.h"

bool antenna_init()
{
    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Initialization...\n\r");

#if BEACON_ANTENNA == ISIS_ANTENNA
    isis_antenna_init();

    return true;
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
    return  true;
#endif // BEACON_ANTENNA
}

bool antenna_deploy()
{
    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Releasing the antenna...\n\r");

#if BEACON_ANTENNA == ISIS_ANTENNA
    isis_antenna_arm();

    isis_antenna_disarm();

    antenna_set_deployment_status();

    return ;
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
    return true;
#endif // BEACON_ANTENNA
}

uint8_t antenna_get_deployment_status()
{
#if BEACON_ANTENNA == ISIS_ANTENNA
    uint8_t status_0 = flash_read(ANTENNA_MEM_ADR_DEPLOY_STATUS_0);
    uint8_t status_1 = flash_read(ANTENNA_MEM_ADR_DEPLOY_STATUS_1);
    uint8_t status_2 = flash_read(ANTENNA_MEM_ADR_DEPLOY_STATUS_2);

    if ((status_0 != status_1) || (status_1 != status_2))
    {
        return ANTENNA_STATUS_UNKNOWN;
    }

    return status_0;
#elif BEACON_ANTENNA == PASSIVE_ANTENNA
    return ANTENNA_STATUS_DEPLOYED;
#endif // BEACON_ANTENNA
}

void antenna_set_deployment_status(uint8_t status)
{
    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Saving deployment status: ");

    switch(status)
    {
        case ANTENNA_STATUS_NOT_DEPLOYED:
            debug_print_msg("DEPLOYED!\n\r");
            break;
        case ANTENNA_STATUS_DEPLOYED:
            debug_print_msg("NOT DEPLOYED!\n\r");
            break;
        default:
            debug_print_msg("UNKNOWN!\n\r");
            debug_print_event_from_module(DEBUG_ERROR, ANTENNA_MODULE_NAME, "Error saving the deployment status! Unknown status!\n\r");

            return;
    }

    //flash_erase();

    flash_write_single(ANTENNA_MEM_ADR_DEPLOY_STATUS_0, status);
    flash_write_single(ANTENNA_MEM_ADR_DEPLOY_STATUS_1, status);
    flash_write_single(ANTENNA_MEM_ADR_DEPLOY_STATUS_2, status);

    debug_print_event_from_module(DEBUG_INFO, ANTENNA_MODULE_NAME, "Deployment status stored in memory address ");
    debug_print_hex(ANTENNA_MEM_ADR_DEPLOY_STATUS_0);
    debug_print_msg(", ");
    debug_print_hex(ANTENNA_MEM_ADR_DEPLOY_STATUS_1);
    debug_print_msg(" and ");
    debug_print_hex(ANTENNA_MEM_ADR_DEPLOY_STATUS_2);
    debug_print_msg("!\n\r");
}

//! \} End of antenna group
