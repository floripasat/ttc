/*
 * debug.h
 * 
 * Copyright (C) 2016, Universidade Federal de Santa Catarina
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file debug.h
 * 
 * \brief Functions for printing messages and bytes over UART.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup debug Debug
 * \ingroup beacon
 * \{
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdint.h>
#include <stdbool.h>

#include "../driverlib/driverlib.h"

/**
 * \brief Debug mode on/off flag.
 * 
 * Turns on/off debug mode.
 * 
 * \{
 */
#define DEBUG_MODE                  false
//! \}

/**
 * \brief Debug mode UART pin map.
 * 
 * \{
 */
#define DEBUG_UART_BASE_ADDRESS     USCI_A1_BASE    /**< Debug UART base address */
#define DEBUG_UART_PORT             GPIO_PORT_P8    /**< Debug UART port = P8 */
#define DEBUG_UART_TX_PIN           GPIO_PIN2       /**< UART TX pin = P8.2 (USCI_A1_TX_BEACON) */
#define DEBUG_UART_RX_PIN           GPIO_PIN3       /**< UART RX pin = P8.3 (USCI_A1_RX_BEACON) */
//! \}

/**
 * \fn debug_Init
 * 
 * \brief Initialization of the debug mode.
 * 
 * After the UART initialization, to show if the debug mode is
 * working, the follow message is transmitted:
 * 
 * FloripaSat-TTC Copyright (C) 2017, Universidade Federal de Santa Catarina;
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions.
 * 
 * Source code: https://github.com/floripasat/ttc
 * Documentation: http://fsat-server.duckdns.org:8000/ttc
 * 
 * FloripaSat debug mode:
 * *************************************
 * 
 * \return UART initialization status. It can be:
 *      - \b STATUS_SUCCES
 *      - \b STATUS_FAIL
 *      .
 */
uint8_t debug_Init();

/**
 * \fn debug_PrintMsg
 * 
 * \brief Prints a message over the UART.
 * 
 * \param msg is the message to be written.
 * 
 * \return None
 */
void debug_PrintMsg(const char *msg);

/**
 * \fn debug_PrintDigit
 * 
 * \brief Prints a integer digit over the debug UART.
 * 
 * \param d is the digit to be written (0 to 9)
 * 
 * \return None
 */
void debug_PrintDigit(uint8_t d);

/**
 * \fn debug_PrintInt8
 * 
 * \brief Prints a 8 bits unsigned integer over the UART.
 * 
 * Example:
 *      - Integer   = 0x65
 *      - Output    = "0x65"
 * 
 * \note
 * The integer is printed in ASCII code.
 * 
 * \param int8 is the integer to be written.
 * 
 * \return None
 */
void debug_PrintInt8(uint8_t int8);

/**
 * \fn debug_PrintInt16
 * 
 * \brief Prints a 16 bits unsigned integer over the UART.
 * 
 * Example:
 *      - Integer   = 0x1865
 *      - Output    = "0x1865"
 * 
 * \note
 * The unsigned integer is printed in ASCII code.
 * 
 * \param int16 is the unsigned integer to be written.
 * 
 * \return None
 */
void debug_PrintInt16(uint16_t int16);

/**
 * \fn debug_PrintByte
 * 
 * \brief Prints a raw byte over the UART.
 * 
 * \param byte is the byte to be printed.
 * 
 * \return None
 */
void debug_PrintByte(uint8_t byte);

/**
 * \fn debug_Abort
 * 
 * \brief Puts the program in an infinite loop.
 * 
 * This function can be used in case of a critical error during debug mode.
 * 
 * \return None
 */
void debug_Abort();

/**
 * \fn debug_UART_Init
 * 
 * \brief Initialization of the debug UART.
 * 
 * UART settings:
 *      - MCU interface = USCI_A1
 *      - Baudrate      = 115200 bps
 *      - Data bits     = 8
 *      - Parity bit    = None
 *      - Stop bits     = 1
 *      .
 * 
 * \return Initialization status. It can be:
 *      - \b STATUS_SUCCESS
 *      - \b STATUS_FAIL
 *      .
 */
uint8_t debug_UART_Init();

#endif // DEBUG_H_

//! \} End of Debug group
