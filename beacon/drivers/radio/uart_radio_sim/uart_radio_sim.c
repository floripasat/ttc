/*
 * uart_radio_sim.c
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina.
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
 * \brief UART Radio Simulation driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 11/06/2017
 * 
 * \addtogroup uart_radio_sim
 * \{
 */

#include <drivers/driverlib/driverlib.h>

#include "uart_radio_sim.h"

uint8_t uart_radio_sim_init(uint8_t init_mode)
{
    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(UART_RADIO_SIM_UART_TX_PORT, RADIO_SIM_UART_TX_PIN);
    
    if (init_mode == UART_RADIO_ONLY_SET_TX_PIN_AS_PERIPHERAL)
    {
        return STATUS_SUCCESS;
    }
    else
    {    
        // Config UART
        USCI_A_UART_initParam uart_params = {0};
        uart_params.selectClockSource   = UART_RADIO_SIM_UART_CLOCK_SOURCE;
        uart_params.clockPrescalar      = UART_RADIO_SIM_UART_CLOCK_PRESCALAR;
        uart_params.firstModReg         = UART_RADIO_SIM_UART_FIRST_MOD_REG;
        uart_params.secondModReg        = UART_RADIO_SIM_UART_SECOND_MOD_REG;
        uart_params.parity              = UART_RADIO_SIM_UART_PARITY;
        uart_params.msborLsbFirst       = UART_RADIO_SIM_UART_ENDIENESS;
        uart_params.numberofStopBits    = UART_RADIO_SIM_UART_STOP_BITS;
        uart_params.uartMode            = UART_RADIO_SIM_UART_MODE;
        uart_params.overSampling        = UART_RADIO_SIM_UART_OVERSAMPLING;
        
        // UART initialization
        if (USCI_A_UART_init(UART_RADIO_SIM_UART_BASE_ADDRESS, &uart_params) == STATUS_SUCCESS)
        {
            // Enable UART module
            USCI_A_UART_enable(UART_RADIO_SIM_UART_BASE_ADDRESS);

            return STATUS_SUCCESS;
        }
        else
        {
            return STATUS_FAIL;
        }
    }
}

void uart_radio_sim_send_data(uint8_t *data, uint16_t size)
{
    uint8_t i = 0;
    for(i=0; i<size; i++)
    {
        USCI_A_UART_transmitData(UART_RADIO_SIM_UART_BASE_ADDRESS, data[i]);
    }
}

//! \} End of uart_radio_sim group
