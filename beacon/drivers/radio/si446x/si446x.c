/*
 * si446x.c
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
 * \file si446x.c
 * 
 * \brief Silicon Labs SI446x driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 01/06/2017
 * 
 * \addtogroup si446x
 * \{
 */

#include <libs/driverlib/driverlib.h>

#include "si446x.h"
#include "si446x_pinmap.h"

uint8_t si446x_init()
{
    return STATUS_SUCCESS;
}

uint8_t si446x_spi_init()
{
    return STATUS_SUCCESS;
}

uint8_t si446x_config()
{
    return STATUS_SUCCESS;
}

void si446x_set_state(uint8_t state)
{
    /*si446x_cmd(SI446X_CMD_CHANGE_STATE);
    
    switch(state)
    {
        case SI446X_STATE_POR:
            si446x_cmd(SI446X_CMD_POWER_UP);
            break;
        case SI446X_STATE_SHUTDOWN:
            GPIO_setOutputHighOnPin(SI446X_SDN_PORT, SI446X_SDN_PIN);
            break;
        case SI446X_STATE_STANDBY:
            break;
        case SI446X_STATE_SLEEP:
            break;
        case SI446X_STATE_SPI_ACTIVE:
            break;
        case SI446X_STATE_READY:
            break;
        case SI446X_STATE_TX:
            si446x_cmd(SI446X_CMD_START_TX);
            break;
        case SI446X_STATE_RX:
            si446x_cmd(SI446X_CMD_START_RX);
            break;
        default:
            GPIO_setOutputHighOnPin(SI446X_SDN_PORT, SI446X_SDN_PIN);
    }*/
}

void si446x_cmd(uint8_t cmd)
{
    
}

void si446x_write_tx_fifo()
{
    
}

void si446x_read_rx_fifo()
{
    
}

//! \} End of si446x group
