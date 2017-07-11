/*
 * init.c
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
 * \file init.c
 * 
 * \brief Initialization routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/06/2017
 * 
 * \addtogroup init
 * \{
 */

#include <config/config.h>
#include <modules/modules.h>
#include <libs/ax25/ax25.h>
#include <libs/ngham/ngham.h>

#include "init.h"

void init_debug()
{
    watchdog_hold();    // Disable watchdog for debug
    
    while(debug_init() != STATUS_SUCCESS)
    {
        
    }
}

void init_watchdog()
{
    watchdog_init();
}

void init_cpu()
{
    
}

void init_memory()
{
    flash_init();
}

void init_status_led()
{
    status_led_init();
}

void init_time(Time *t)
{
    time_reset(t);
}

void init_timer()
{
    timer_init();
}

void init_antenna()
{
    while(antenna_init() != STATUS_SUCCESS)
    {
        
    }
}

void init_eps_com()
{
    while(eps_com_init() != STATUS_SUCCESS)
    {
        
    }
}

void init_obdh_com()
{
    while(obdh_com_init() != STATUS_SUCCESS)
    {
        
    }
}

void init_radio()
{
    while(radio_init() != STATUS_SUCCESS)
    {
        
    }
}

void init_rf_switch()
{
    rf_switch_init();
}

void init_pa()
{
    while(pa_init() != STATUS_SUCCESS)
    {
        
    }
}

void init_protocols()
{
    ngham_Init();
}

//! \} End of init group
