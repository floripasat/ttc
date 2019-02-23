/*
 * debug.c
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * \brief Debug functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 23/09/2016
 * 
 * \addtogroup debug
 * \{
 */

#include <math.h>

#include <drivers/driverlib/driverlib.h>
#include <system/time/time.h>
#include <version.h>

#include "debug.h"
#include "debug_config.h"

bool debug_init()
{
    if (debug_uart_init())
    {
        debug_print_license_msg();

        debug_print_splash_screen();

        debug_print_msg("Version:\t");
        debug_print_firmware_version();
        debug_print_msg("\n\r");

        debug_print_msg("Status:\t\t");
        debug_print_msg(FIRMWARE_STATUS);
        debug_print_msg("\n\r");

        debug_print_msg("Author:\t\t");
        debug_print_msg(FIRMWARE_AUTHOR_NAME);
        debug_print_msg(" <");
        debug_print_msg(FIRMWARE_AUTHOR_EMAIL);
        debug_print_msg(">");
        debug_print_msg("\n\n\n\r");

        return true;
    }
    else
    {
        return false;
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

void debug_print_dec(uint32_t dec)
{
    if (dec == 0)
    {
        debug_print_digit(0);
    }
    else
    {
        uint8_t dec_str[10];                        // 32-bits = decimal with 10 digits

        uint8_t digits = log10(dec) + 1;

        uint8_t i = 0;
        for(i=0; i<digits; ++i, dec /= 10)
        {
            dec_str[i] = dec % 10;
        }

        uint8_t j = 0;
        for(j=i; j>0; j--)
        {
            debug_print_digit(dec_str[j-1]);
        }
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

void debug_print_system_time()
{
    debug_print_msg("[ ");
    debug_print_dec(1000*time_get_seconds());
    debug_print_msg(" ]");
}

void debug_print_license_msg()
{
    debug_print_msg("FloripaSat-Beacon Copyright (C) 2017-2019, Universidade Federal de Santa Catarina;\n\r");
    debug_print_msg("This program comes with ABSOLUTELY NO WARRANTY.\n\r");
    debug_print_msg("This is free software, and you are welcome to redistribute it\n\r");
    debug_print_msg("under certain conditions.\n\n\r");
    debug_print_msg("Source code: https://github.com/floripasat/ttc\n\r");
    debug_print_msg("Documentation: https://github.com/floripasat/ttc/wiki\n\r");
}

void debug_print_splash_screen()
{
    debug_print_msg("                                                         \n\r");
    debug_print_msg("                                                         \n\r");
    debug_print_msg(".........................................................\n\r");
    debug_print_msg(".........................................................\n\r");
    debug_print_msg(".........................................................\n\r");
    debug_print_msg("......  _____ ____        _      _____ _____ ____  ......\n\r");
    debug_print_msg("...... |  ___/ ___|  __ _| |_   |_   _|_   _/ ___| ......\n\r");
    debug_print_msg("...... | |_  \\___ \\ / _` | __|____| |   | || |     ......\n\r");
    debug_print_msg("...... |  _|  ___) | (_| | ||_____| |   | || |___  ......\n\r");
    debug_print_msg("...... |_|   |____/ \\__,_|\\__|    |_|   |_| \\____| ......\n\r");
    debug_print_msg("......                                             ......\n\r");
    debug_print_msg(".........................................................\n\r");
    debug_print_msg(".........................................................\n\r");
    debug_print_msg("                                                         \n\r");
    debug_print_msg("                                                         \n\r");
}

void debug_print_firmware_version()
{
    debug_print_msg("[ ");
    debug_print_msg(FIRMWARE_VERSION);
    debug_print_msg(" ]");
}

void debug_abort()
{
    while(1)
    {
        
    }
}

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

//! \} End of debug group
