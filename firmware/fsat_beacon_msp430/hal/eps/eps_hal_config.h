/*
 * eps_hal_config.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * \brief EPS communication configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.7
 * 
 * \date 13/07/2017
 * 
 * \defgroup eps_hal_config Configuration
 * \ingroup eps_hal
 * \{
 */

#ifndef EPS_HAL_CONFIG_H_
#define EPS_HAL_CONFIG_H_

#include <config/config.h>

#define EPS_HAL_MODULE_NAME                 "EPS"

// Timeout timer base address
#define EPS_HAL_TIMEOUT_TIMER_BASE          TIMER_A0_BASE

// Config UART (4800 bps, no parity, 1 stop bit, LSB first)
#define EPS_HAL_CLOCK_SOURCE                USCI_A_UART_CLOCKSOURCE_SMCLK
#define EPS_HAL_UART_CLOCK_PRESCALAR        52                                              /**< Clock = 16 MHz, Baudrate = 4800 bps ([1] http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html). */
#define EPS_HAL_UART_FIRST_MOD_REG          1                                               /**< Clock = 16 MHz, Baudrate = 4800 bps (See [1]). */
#define EPS_HAL_UART_SECOND_MOD_REG         0                                               /**< Clock = 16 MHz, Baudrate = 4800 bps (See [1]). */
#define EPS_HAL_UART_PARITY                 USCI_A_UART_NO_PARITY
#define EPS_HAL_UART_ENDIENESS              USCI_A_UART_LSB_FIRST
#define EPS_HAL_UART_STOP_BITS              USCI_A_UART_ONE_STOP_BIT
#define EPS_HAL_UART_MODE                   USCI_A_UART_MODE
#define EPS_HAL_UART_OVERSAMPLING           USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION    /**< Clock = 16 MHz, Baudrate = 4800 bps (See [1]). */
#define EPS_HAL_UART_ISR_VECTOR             USCI_A0_VECTOR

#endif // EPS_CONFIG_H_

//! \} End of eps_hal_config group
