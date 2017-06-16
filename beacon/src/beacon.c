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
#include "flags.h"
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

    init_time();
    
    init_timer();
    
    init_antenna();
    
    init_eps_com();
    
    init_obdh_com();
    
    init_radio();
    
#if BEACON_RF_SWITCH != HW_NONE
    init_rf_switch();
#endif // BEACON_RF_SWITCH

#if BEACON_PA != HW_NONE    
    init_pa();
#endif // BEACON_PA
    
    init_protocols();
}

void beacon_run()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Running...\n");
#endif // DEBUG_MODE
    
#if BEACON_MODE == FLIGHT_MODE
    if (!antenna_is_released())
    {
        task_antenna_deployment();
    }
#endif // BEACON_MODE
    
    while(1)
    {
        if (flags.hibernation == false)
        {
            if (flags.can_transmit == true)
            {
                task_transmit_packet();
            }
        }
        else
        {
            if (task_check_elapsed_time(time.hour, beacon.hibernation_mode_initial_time.hour, HOURS) >= BEACON_HIBERNATION_PERIOD_HOURS)
            {
                if (task_check_elapsed_time(time.minute, beacon.hibernation_mode_initial_time.minute, MINUTES) >= BEACON_HIBERNATION_PERIOD_MINUTES)
                {
                    if (task_check_elapsed_time(time.seconds, beacon.hibernation_mode_initial_time.minute, SECONDS) >= BEACON_HIBERNATION_PERIOD_SECONDS)
                    {
                        task_leave_hibernation();
                    }
                }
            }
        }
        
        uint8_t second_marker = time.second;
        while(task_check_elapsed_time(second_marker, time.second, SECONDS) < BEACON_TX_PERIOD_SEC)
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
