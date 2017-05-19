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

#include "../driverlib/driverlib.h"

// P2.5 = UART0_EPS_TX_BEACON_RX
#define EPS_UART_BASE_ADDRESS           USCI_A0_BASE
#define EPS_UART_PORT                   GPIO_PORT_P2
#define EPS_UART_RX_PIN                 GPIO_PIN5

#define EPS_UART_PKT_LEN                0x04
#define EPS_UART_SOD                    0x7E
#define EPS_UART_BYTE_COUNTER_POS_SOD   0x00
#define EPS_UART_BYTE_COUNTER_POS_CRC   EPS_UART_PKT_LEN+1

#define EPS_UART_DEFAULT_DATA_MSB       0x07
#define EPS_UART_DEFAULT_DATA_LSB       0xFD
#define EPS_UART_CRC_INITIAL_VALUE      0x00
#define EPS_UART_CRC_POLY               0x07

/**
 * \brief UART-EPS interruption variables.
 * 
 * \{
 */
extern uint8_t eps_uart_received_byte;                  /**< Byte buffer. */
extern uint8_t eps_uart_byte_counter;                   /**< Received packet byte counter. */
extern uint8_t eps_data_buffer[EPS_UART_PKT_LEN + 1];   /**< Packet buffer. */
extern uint8_t eps_data[EPS_UART_PKT_LEN + 1];          /**< EPS data storage. */
//! \}

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

#endif // UART_EPS_H_

//! \} End of UART-EPS group
