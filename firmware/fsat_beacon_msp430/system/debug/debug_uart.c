/*
 * debug_uart.c
 * 
 * Copyright (C) 2019, Universidade Federal de Santa Catarina
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
 * \brief Debug UART implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.4
 * 
 * \date 22/02/2019
 * 
 * \defgroup debug_uart UART
 * \ingroup debug
 * \{
 */

#include <drivers/driverlib/driverlib.h>

#include "debug.h"
#include "debug_config.h"

bool debug_uart_init()
{
    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(DEBUG_UART_PORT, DEBUG_UART_TX_PIN + DEBUG_UART_RX_PIN);

    // Config UART (115200 bps, no parity, 1 stop bit, LSB first)
    USCI_A_UART_initParam uart_params = {0};

    uart_params.selectClockSource   = DEBUG_UART_CLOCK_SOURCE;
    uart_params.clockPrescalar      = DEBUG_UART_CLOCK_PRESCALAR;   // Clock = 4 MHz, Baudrate = 115200 bps	([1] http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html)
    uart_params.firstModReg         = DEBUG_UART_FIRST_MOD_REG;     // Clock = 4 MHz, Baudrate = 115200 bps (See [1])
    uart_params.secondModReg        = DEBUG_UART_SECONDS_MOD_REG;   // Clock = 4 MHz, Baudrate = 115200 bps (See [1])
    uart_params.parity              = DEBUG_UART_PARITY;
    uart_params.msborLsbFirst       = DEBUG_UART_ENDIANESS;
    uart_params.numberofStopBits    = DEBUG_UART_STOP_BITS;
    uart_params.uartMode            = DEBUG_UART_MODE;
    uart_params.overSampling        = DEBUG_UART_OVER_SAMPLING;     // Clock = 4 MHz, Baudrate = 115200 bps (See [1])

    // UART initialization
    if (USCI_A_UART_init(DEBUG_UART_BASE_ADDRESS, &uart_params) == STATUS_SUCCESS)
    {
        // Enable UART module
        USCI_A_UART_enable(DEBUG_UART_BASE_ADDRESS);

        return true;
    }
    else
    {
        return false;
    }
}

void debug_uart_write_byte(uint8_t byte)
{
    USCI_A_UART_transmitData(DEBUG_UART_BASE_ADDRESS, byte);
}

//! \} End of debug_uart group
