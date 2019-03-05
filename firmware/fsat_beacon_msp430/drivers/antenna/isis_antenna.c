/*
 * isis_antenna_implementation.h
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
 * \brief ISIS antenna driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 20/09/2017
 * 
 * \addtogroup isis_antenna
 * \{
 */

#include <drivers/driverlib/driverlib.h>
#include <system/debug/debug.h>

#include "isis_antenna.h"
#include "isis_antenna_config.h"
#include "isis_antenna_cmds.h"

void isis_antenna_init()
{
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Initializing...\n\r");

    isis_antenna_i2c_init();
}

bool isis_antenna_is_released()
{
    
}

bool isis_antenna_release()
{
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Releasing...\n\r");
}

void isis_antenna_arm()
{
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Arming...\n\r");

    isis_antenna_i2c_write_byte(ISIS_ANTENNA_CMD_ARM);

    isis_antenna_delay_ms(100);
}

void isis_antenna_disarm()
{
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Disarming...\n\r");

    isis_antenna_i2c_write_byte(ISIS_ANTENNA_CMD_DISARM);

    isis_antenna_delay_ms(100);
}

void isis_antenna_start_sequential_deploy(uint8_t sec)
{
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Executing sequential deployment...\n\r");

    uint8_t cmd[2];

    cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_SEQUENTIAL;
    cmd[1] = sec;

    isis_antenna_i2c_write_data(cmd, 2);

    isis_antenna_delay_ms(100);
}

void isis_antenna_start_independent_deploy(uint8_t ant, uint8_t sec, uint8_t ovr)
{
    uint8_t cmd[2];

    cmd[1] = sec;

    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Executing independent deployment of ");

    if (ovr == 1)
    {
        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                debug_print_msg(" ANTENNA 1 with OVERRIDE...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_2:
                debug_print_msg(" ANTENNA 2 with OVERRIDE...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_3:
                debug_print_msg(" ANTENNA 3 with OVERRIDE...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_4:
                debug_print_msg(" ANTENNA 4 with OVERRIDE...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_4_WITH_OVERRIDE;
                break;
            default:
                cmd[0] = ISIS_ANTENNA_CMD_DISARM;
        }
    }
    else
    {
        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                debug_print_msg(" ANTENNA 1...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1;
                break;
            case ISIS_ANTENNA_ANT_2:
                debug_print_msg(" ANTENNA 2...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2;
                break;
            case ISIS_ANTENNA_ANT_3:
                debug_print_msg(" ANTENNA 3...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3;
                break;
            case ISIS_ANTENNA_ANT_4:
                debug_print_msg(" ANTENNA 4...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_4;
                break;
            default:
                cmd[0] = ISIS_ANTENNA_CMD_DISARM;
        }
    }

    isis_antenna_i2c_write_data(cmd, 2);

    isis_antenna_delay_ms(100);
}

static uint16_t isis_antenna_read_deployment_status()
{
    uint16_t status = ISIS_ANTENNA_STATUS_MASK;     // Initial state (all not deployed and disarmed
    
    isis_antenna_i2c_write_byte(ISIS_ANTENNA_CMD_REPORT_DEPLOY_STATUS);
    
    isis_antenna_delay_ms(1000);
    
    uint8_t status_bytes[2];
    
    isis_antenna_i2c_read_data(status_bytes, 2);
    
    status = (uint16_t)(status_bytes[1] << 8) | status_bytes[0];
    
    return status;
}

//! \} End of isis_antenna group
