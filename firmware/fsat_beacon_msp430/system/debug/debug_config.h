/*
 * debug_config.h
 * 
 * Copyright (C) 2019, Universidade Federal de Santa Catarina
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
 * \brief Debug configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.10
 * 
 * \date 22/02/2019
 * 
 * \defgroup debug_config Configuration
 * \ingroup debug
 * \{
 */

#ifndef DEBUG_CONFIG_H_
#define DEBUG_CONFIG_H_

#include <config/config.h>
#include <config/pinmap.h>

#include "debug.h"

// UART
#define DEBUG_UART_BASE_ADDRESS         SYS_DEBUG_UART_BASE_ADDRESS
#define DEBUG_UART_PORT                 SYS_DEBUG_UART_PORT
#define DEBUG_UART_TX_PIN               SYS_DEBUG_UART_TX_PIN
#define DEBUG_UART_RX_PIN               SYS_DEBUG_UART_RX_PIN

#define DEBUG_UART_CLOCK_SOURCE         SYS_DEBUG_UART_CLOCK_SOURCE
#define DEBUG_UART_CLOCK_PRESCALAR      SYS_DEBUG_UART_CLOCK_PRESCALAR      
#define DEBUG_UART_FIRST_MOD_REG        SYS_DEBUG_UART_FIRST_MOD_REG
#define DEBUG_UART_SECONDS_MOD_REG      SYS_DEBUG_UART_SECONDS_MOD_REG
#define DEBUG_UART_PARITY               SYS_DEBUG_UART_PARITY
#define DEBUG_UART_ENDIANESS            SYS_DEBUG_UART_ENDIANESS
#define DEBUG_UART_STOP_BITS            SYS_DEBUG_UART_STOP_BITS
#define DEBUG_UART_MODE                 SYS_DEBUG_UART_MODE
#define DEBUG_UART_OVER_SAMPLING        SYS_DEBUG_UART_OVER_SAMPLING

// Debug messages colors
#define DEBUG_SYSTEM_TIME_COLOR         DEBUG_COLOR_GREEN
#define DEBUG_MODULE_NAME_COLOR         DEBUG_COLOR_MAGENTA
#define DEBUG_WARNING_COLOR             DEBUG_COLOR_YELLOW
#define DEBUG_ERROR_COLOR               DEBUG_COLOR_RED

#endif // DEBUG_CONFIG_H_

//! \} End of debug_config group
