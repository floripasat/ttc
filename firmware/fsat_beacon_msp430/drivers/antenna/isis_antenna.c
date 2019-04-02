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
 * \version 0.4.0
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

void isis_antenna_arm()
{
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Arming...\n\r");

    isis_antenna_i2c_write_byte(ISIS_ANTENNA_CMD_ARM);

    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Arming command transmitted!\n\r");

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
    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Executing sequential deployment (");
    debug_print_dec(sec);
    debug_print_msg(" sec)...\n\r");

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

    debug_print_event_from_module(DEBUG_INFO, ISIS_ANTENNA_MODULE_NAME, "Executing independent deployment (");
    debug_print_dec(sec);
    debug_print_msg(" sec) of ");

    if (ovr == ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE)
    {
        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                debug_print_msg("ANTENNA 1 with OVERRIDE...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_2:
                debug_print_msg("ANTENNA 2 with OVERRIDE...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_3:
                debug_print_msg("ANTENNA 3 with OVERRIDE...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_4:
                debug_print_msg("ANTENNA 4 with OVERRIDE...\n\r");
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
                debug_print_msg("ANTENNA 1...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1;
                break;
            case ISIS_ANTENNA_ANT_2:
                debug_print_msg("ANTENNA 2...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2;
                break;
            case ISIS_ANTENNA_ANT_3:
                debug_print_msg("ANTENNA 3...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3;
                break;
            case ISIS_ANTENNA_ANT_4:
                debug_print_msg("ANTENNA 4...\n\r");
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_4;
                break;
            default:
                cmd[0] = ISIS_ANTENNA_CMD_DISARM;
        }
    }

    isis_antenna_i2c_write_data(cmd, 2);

    isis_antenna_delay_ms(100);
}

isis_antenna_status_t isis_antenna_read_deployment_status()
{
    uint16_t status_code = ISIS_ANTENNA_STATUS_MASK;    // Initial state

    isis_antenna_i2c_write_byte(ISIS_ANTENNA_CMD_REPORT_DEPLOY_STATUS);

    isis_antenna_delay_ms(1000);

    uint8_t status_bytes[2];

    isis_antenna_i2c_read_data(status_bytes, 2);

    status_code = (uint16_t)(status_bytes[1] << 8) | status_bytes[0];

    isis_antenna_status_t status;

    status.antenna_1.status     = (status_code >> 15) & 0x01;
    status.antenna_1.timeout    = (status_code >> 14) & 0x01;
    status.antenna_1.burning    = (status_code >> 13) & 0x01;
    status.antenna_2.status     = (status_code >> 11) & 0x01;
    status.antenna_2.timeout    = (status_code >> 10) & 0x01;
    status.antenna_2.burning    = (status_code >> 9) & 0x01;
    status.ignoring_switches    = (status_code >> 8) & 0x01;
    status.antenna_3.status     = (status_code >> 7) & 0x01;
    status.antenna_3.timeout    = (status_code >> 6) & 0x01;
    status.antenna_3.burning    = (status_code >> 5) & 0x01;
    status.independent_burn     = (status_code >> 4) & 0x01;
    status.antenna_4.status     = (status_code >> 3) & 0x01;
    status.antenna_4.timeout    = (status_code >> 2) & 0x01;
    status.antenna_4.burning    = (status_code >> 1) & 0x01;
    status.armed                = (status_code >> 0) & 0x01;

    return status;
}

uint8_t isis_antenna_get_antenna_status(uint8_t ant)
{
    isis_antenna_status_t status = isis_antenna_read_deployment_status();

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:
            return status.antenna_1.status;
        case ISIS_ANTENNA_ANT_2:
            return status.antenna_2.status;
        case ISIS_ANTENNA_ANT_3:
            return status.antenna_3.status;
        case ISIS_ANTENNA_ANT_4:
            return status.antenna_4.status;
        default:
            debug_print_event_from_module(DEBUG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the status of an antenna! Unknown antenna number!");
            return ISIS_ANTENNA_STATUS_DEPLOYED;
    }
}

uint8_t isis_antenna_get_antenna_timeout(uint8_t ant)
{
    isis_antenna_status_t status = isis_antenna_read_deployment_status();

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:
            return status.antenna_1.timeout;
        case ISIS_ANTENNA_ANT_2:
            return status.antenna_2.timeout;
        case ISIS_ANTENNA_ANT_3:
            return status.antenna_3.timeout;
        case ISIS_ANTENNA_ANT_4:
            return status.antenna_4.timeout;
        default:
            debug_print_event_from_module(DEBUG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the timeout flag of an antenna! Unknown antenna number!");
            return ISIS_ANTENNA_OTHER_CAUSE;
    }
}

uint8_t isis_antenna_get_burning(uint8_t ant)
{
    isis_antenna_status_t status = isis_antenna_read_deployment_status();

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:
            return status.antenna_1.burning;
        case ISIS_ANTENNA_ANT_2:
            return status.antenna_2.burning;
        case ISIS_ANTENNA_ANT_3:
            return status.antenna_3.burning;
        case ISIS_ANTENNA_ANT_4:
            return status.antenna_4.burning;
        default:
            debug_print_event_from_module(DEBUG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the burning flag of an antenna! Unknown antenna number!");
            return ISIS_ANTENNA_BURN_INACTIVE;
    }
}

uint8_t isis_antenna_get_arming_status()
{
    return isis_antenna_read_deployment_status().armed;
}

//! \} End of isis_antenna group
