/*
 * debug.c
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
 * \file debug.c
 * 
 * \brief Debug functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \addtogroup debug
 * \{
 */

#include <config/config.h>
#include <libs/driverlib/driverlib.h>

#include "debug.h"

uint8_t debug_init()
{
    if (debug_uart_init() == STATUS_SUCCESS)
    {
        char debug_msg[] = "FloripaSat-Beacon Copyright (C) 2017, Federal University of Santa Catarina;\n"
                           "This program comes with ABSOLUTELY NO WARRANTY.\n"
                           "This is free software, and you are welcome to redistribute it\n"
                           "under certain conditions.\n\n"
                           "Source code: https://github.com/floripasat/ttc\n"
                           "Documentation: http://fsat-server.duckdns.org:8000/ttc\n\n"
                           "FloripaSat Beacon debug mode:\n"
                           "*************************************\n\n";
        uint16_t i = 0;
        for(i=0;i<sizeof(debug_msg)-1;i++)
        {
            USCI_A_UART_transmitData(DEBUG_UART_BASE_ADDRESS, debug_msg[i]);
        }
        
        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

void debug_print_msg(const char *msg)
{
    uint8_t i = 0;
    while(msg[i] != '\0')
    {
        USCI_A_UART_transmitData(DEBUG_UART_BASE_ADDRESS, msg[i]);
        i++;
    }
}

void debug_print_digit(uint8_t digit)
{
    if (digit < 0x0A)
    {
        USCI_A_UART_transmitData(DEBUG_UART_BASE_ADDRESS, digit + 0x30);  // 0x30 = ascii 0
    }
    else if (digit <= 0x0F)
    {
        USCI_A_UART_transmitData(DEBUG_UART_BASE_ADDRESS, digit + 0x37);  // 0x37 = ascii 7
    }
    else
    {
        USCI_A_UART_transmitData(DEBUG_UART_BASE_ADDRESS, 'N');
    }
}

void debug_print_int8(uint8_t int8)
{
    debug_print_msg("0x");
    
    debug_print_digit((uint8_t)(int8 >> 4));
    debug_print_digit((uint8_t)(int8 & 0x0F));
}

void debug_print_int16(uint16_t int16)
{
    debug_print_msg("0x");
    
    debug_print_digit((uint8_t)(int16 >> 12));
    debug_print_digit((uint8_t)(int16 >> 8) & 0x0F);
    debug_print_digit((uint8_t)(int16 >> 4) & 0x0F);
    debug_print_digit((uint8_t)(int16 & 0x0F));
}

void debug_print_byte(uint8_t byte)
{
    USCI_A_UART_transmitData(DEBUG_UART_BASE_ADDRESS, byte);
}

void debug_abort()
{
    while(1)
    {
        
    }
}

uint8_t debug_uart_init()
{
    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(DEBUG_UART_PORT, DEBUG_UART_TX_PIN + DEBUG_UART_RX_PIN);
    
    // Config UART (115200 bps, no parity, 1 stop bit, LSB first)
    USCI_A_UART_initParam uart_params = {0};
    uart_params.selectClockSource   = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uart_params.clockPrescalar      = 8;		// Clock = 1 MHz, Baudrate = 115200 bps	([1] http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html)
    uart_params.firstModReg         = 0;		// Clock = 1 MHz, Baudrate = 115200 bps (See [1])
    uart_params.secondModReg        = 6;		// Clock = 1 MHz, Baudrate = 115200 bps (See [1])
    uart_params.parity              = USCI_A_UART_NO_PARITY;
    uart_params.msborLsbFirst       = USCI_A_UART_LSB_FIRST;
    uart_params.numberofStopBits    = USCI_A_UART_ONE_STOP_BIT;
    uart_params.uartMode            = USCI_A_UART_MODE;
    uart_params.overSampling        = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;    // Clock = 1 MHz, Baudrate = 115200 bps (See [1])

    // UART initialization
    if (USCI_A_UART_init(DEBUG_UART_BASE_ADDRESS, &uart_params) == STATUS_SUCCESS)
    {
        // Enable UART module
        USCI_A_UART_enable(DEBUG_UART_BASE_ADDRESS);

        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

//! \} End of debug group
