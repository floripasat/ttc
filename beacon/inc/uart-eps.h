/*
 * uart-eps.h
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
 * \file uart-eps.h
 * 
 * \brief Functions to receive data from EPS module
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup eps_uart UART-EPS
 * \ingroup beacon
 * \{
 */

#ifndef UART_EPS_H_
#define UART_EPS_H_

#include <stdint.h>
#include "debug.h"

// P2.5 = UART0_EPS_TX_BEACON_RX
#define EPS_UART_PORT       GPIO_PORT_P2
#define EPS_UART_RX_PIN     GPIO_PIN5

#define EPS_UART_SOD                    0x7E
#define EPS_UART_BYTE_COUNTER_POS_SOD   0x00
#define EPS_UART_BYTE_COUNTER_POS_CRC   0x05

/**
 * \fn eps_UART_Init
 * 
 * \brief Initialization of the EPS UART (USCI_A0)
 * 
 * \return Initialization status. It can be:
 *      - \b STATUS_SUCCESS
 *      - \b STATUS_FAIL
 *      .
 */
uint8_t eps_UART_Init();

/**
 * \fn eps_UART_crc8
 * 
 * \brief crc8 checksum.
 * 
 * \param initial_value is the initial value of the crc8.
 * \param polynomial is the crc8 polynomial.
 * \param data is data to calculate the crc8.
 * \param len is lenght of the data.
 * 
 * \return The crc8 value of the given data.
 */
uint8_t eps_UART_crc8(uint8_t initial_value, uint8_t polynomial, uint8_t *data, uint8_t len);

#endif // UART_EPS_H_

//! \} End of UART-EPS group
