/*
 * isis_antenna_implementation.h
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
 * \file isis_antenna.c
 * 
 * \brief ISIS antenna driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 20/09/2017
 * 
 * \addtogroup isis_antenna
 * \{
 */

#include <drivers/driverlib/driverlib.h>

#include "isis_antenna.h"
#include "isis_antenna_cmds.h"
#include "isis_antenna_i2c.h"
#include "isis_antenna_delay.h"

void isis_antenna_init()
{
    isis_antenna_i2c_init();
}

bool isis_antenna_is_released()
{
    
}

bool isis_antenna_release()
{
    
}

void isis_antenna_arm()
{    
    isis_antenna_i2c_write_byte(ISIS_ANTENNA_CMD_ARM);
    
    isis_antenna_delay_ms(100);
}

void isis_antenna_disarm()
{
    isis_antenna_i2c_write_byte(ISIS_ANTENNA_CMD_DISARM);
    
    isis_antenna_delay_ms(100);
}

void isis_antenna_start_sequential_deploy(uint8_t sec)
{
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
    
    if (ovr == 1)
    {
        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_2:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_3:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_4:
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
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1;
                break;
            case ISIS_ANTENNA_ANT_2:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2;
                break;
            case ISIS_ANTENNA_ANT_3:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3;
                break;
            case ISIS_ANTENNA_ANT_4:
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
