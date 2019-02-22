/*
 * debug.h
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
 * \brief Functions for printing messages and bytes over UART port.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.2
 * 
 * \date 23/09/2016
 * 
 * \defgroup debug Debug
 * \ingroup system
 * \{
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdint.h>
#include <stdbool.h>

/**
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
 *      - TRUE
 *      - FALSE
 *      .
 */
bool debug_init();

/**
 * \brief Prints a message over the UART.
 * 
 * \param msg is the message to be written.
 * 
 * \return None.
 */
void debug_print_msg(const char *msg);

/**
 * \brief Prints a integer digit over the debug UART.
 * 
 * \param d is the digit to be written (0 to 9)
 * 
 * \return None.
 */
void debug_print_digit(uint8_t d);

/**
 * \brief Prints a decimal number over the debug UART.
 *
 * \param[in] dec is the decimal number to print.
 *
 * \return None.
 */
void debug_print_dec(uint32_t dec);

/**
 * \brief Prints a 8 bits unsigned integer over the UART.
 * 
 * Example:
 *      - Integer   = 0x65
 *      - Output    = "0x65"
 * 
 * \note The integer is printed in ASCII code.
 * 
 * \param int8 is the integer to be written.
 * 
 * \return None.
 */
void debug_print_int8(uint8_t int8);

/**
 * \brief Prints a 16 bits unsigned integer over the UART.
 * 
 * Example:
 *      - Integer   = 0x1865
 *      - Output    = "0x1865"
 * 
 * \note The unsigned integer is printed in ASCII code.
 * 
 * \param int16 is the unsigned integer to be written.
 * 
 * \return None.
 */
void debug_print_int16(uint16_t int16);

/**
 * \brief Prints a raw byte over the UART.
 * 
 * \param byte is the byte to be printed.
 * 
 * \return None.
 */
void debug_print_byte(uint8_t byte);

/**
 * \brief Prints the system time in milliseconds.
 *
 * \return None.
 */
void debug_print_system_time();

/**
 * \brief Prints the license text and genreal firmware information.
 *
 * \return None.
 */
void debug_print_license_msg();

/**
 * \brief Prints the splash screen of the firmware.
 *
 * \return None.
 */
void debug_print_splash_screen();

/**
 * \brief Writes the current firmware version.
 *
 * \return None.
 */
void debug_print_firmware_version();

/**
 * \brief Puts the program in an infinite loop.
 * 
 * This function can be used in case of a critical error during debug mode.
 * 
 * \return None.
 */
void debug_abort();

/**
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
 *      - TRUE
 *      - FALSE
 *      .
 */
bool debug_uart_init();

#endif // DEBUG_H_

//! \} End of debug group
