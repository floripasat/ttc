/*
 * obdh_com.c
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
 * \file obdh_com.c
 * 
 * \brief Implementation of the functions to communicate with the OBDH module.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \addtogroup obdh_com
 * \{
 */

#include <config/config.h>
#include <modules/modules.h>
#include <libs/driverlib/driverlib.h>

#include "obdh_com.h"

void obdh_com_init()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Initializing OBDH communication peripherals... ");
#endif // BEACON_MODE

    while(obdh_com_spi_init() != STATUS_SUCCESS)
    {
        
    }

#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("SUCCESS!\n");
#endif // BEACON_MODE
}

void obdh_com_send_cmd(uint8_t cmd)
{
    switch(cmd)
    {
        case OBDH_COM_BEACON_TX_ON:
            break;
        default:
            break;
    };
}

static uint8_t obdh_com_spi_init()
{
    
}

static void obdh_com_receive_data()
{
    
}

//! \} End of obdh_com group
