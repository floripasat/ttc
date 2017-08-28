/*
 * beacon.c
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
 * \file beacon.c
 * 
 * \brief Beacon implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/06/2017
 * 
 * \addtogroup beacon
 * \{
 */

#include <msp430.h>
#include <stdbool.h>
#include <config/config.h>
#include <modules/modules.h>

#include "beacon.h"
#include "init.h"
#include "tasks.h"

Beacon beacon;

void beacon_init()
{
#if BEACON_MODE == DEBUG_MODE
    init_debug();
#else
    init_watchdog();
#endif // DEBUG_MODE

    init_cpu();

    init_memory();

#if BEACON_MODE != FLIGHT_MODE
    init_status_led();
#endif // FLIGHT_MODE

    init_time(&beacon.time);
    
    init_timer(&beacon.time);
    
    init_antenna();
    
    init_eps_com(&beacon);
    
    init_obdh_com(&beacon.obdh);
    
    init_radio();
    
#if BEACON_RF_SWITCH != HW_NONE
    init_rf_switch();
#endif // BEACON_RF_SWITCH

#if BEACON_PA != HW_NONE    
    init_pa();
#endif // BEACON_PA
    
    init_protocols();
    
    // Starts the time control timer
    timer_start();
    
    beacon.flags.hibernation    = false;
    beacon.flags.can_transmit   = true;
    beacon.flags.transmitting   = false;
    beacon.energy_level         = SATELLITE_ENERGY_LEVEL_5;
}

void beacon_run()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Running...\n");
#elif BEACON_MODE == FLIGHT_MODE
    if (!antenna_is_released())
    {
        task_antenna_deployment();
    }
#endif // BEACON_MODE
    
    __enable_interrupt();
    
    while(1)
    {
        if (beacon.flags.hibernation == false)
        {
            if (beacon.flags.can_transmit == true)
            {
                task_transmit_packet(&beacon);
                task_set_energy_level(&beacon);
            }
        }
        else
        {
            if (task_check_elapsed_time(beacon.time.hour, beacon.hibernation_mode_initial_time.hour, HOURS) >= BEACON_HIBERNATION_PERIOD_HOURS)
            {
                if (task_check_elapsed_time(beacon.time.minute, beacon.hibernation_mode_initial_time.minute, MINUTES) >= BEACON_HIBERNATION_PERIOD_MINUTES)
                {
                    if (task_check_elapsed_time(beacon.time.second, beacon.hibernation_mode_initial_time.minute, SECONDS) >= BEACON_HIBERNATION_PERIOD_SECONDS)
                    {
                        task_leave_hibernation();
                    }
                }
            }
        }
        
        uint8_t second_marker = beacon.time.second;
        while(task_check_elapsed_time(second_marker, beacon.time.second, SECONDS) < task_get_tx_period())
        {
            watchdog_reset_timer();
            
            task_enter_low_power_mode();
        }
    }
}

void beacon_deinit()
{
    
}

void beacon_shutdown()
{
    //_BIS_SR(LPM4_5_bits);
}

//! \} End of beacon group
