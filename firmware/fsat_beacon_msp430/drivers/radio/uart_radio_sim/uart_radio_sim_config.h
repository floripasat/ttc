/*
 * uart_radio_sim_config.h
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
 * \brief UART Radio Simulation driver configuration parameters.
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

#ifndef UART_RADIO_SIM_CONFIG_H_
#define UART_RADIO_SIM_CONFIG_H_

#include <config/config.h>

// UART-RADIO-SIM pin map
#define UART_RADIO_SIM_UART_USCI                        RADIO_SIM_UART_USCI
#define UART_RADIO_SIM_UART_BASE_ADDRESS                RADIO_SIM_UART_BASE_ADDRESS
#define UART_RADIO_SIM_UART_TX_PORT                     RADIO_SIM_UART_TX_PORT
#define UART_RADIO_SIM_UART_TX_PIN                      RADIO_SIM_UART_TX_PIN

#define UART_RADIO_SIM_UART_CLOCK_SOURCE                RADIO_SIM_UART_CLOCK_SOURCE
#define UART_RADIO_SIM_UART_CLOCK_PRESCALAR             RADIO_SIM_UART_CLOCK_PRESCALAR
#define UART_RADIO_SIM_UART_FIRST_MOD_REG               RADIO_SIM_UART_FIRST_MOD_REG
#define UART_RADIO_SIM_UART_SECOND_MOD_REG              RADIO_SIM_UART_SECOND_MOD_REG
#define UART_RADIO_SIM_UART_PARITY                      RADIO_SIM_UART_PARITY
#define UART_RADIO_SIM_UART_ENDIENESS                   RADIO_SIM_UART_ENDIENESS
#define UART_RADIO_SIM_UART_STOP_BITS                   RADIO_SIM_UART_STOP_BITS
#define UART_RADIO_SIM_UART_MODE                        RADIO_SIM_UART_MODE
#define UART_RADIO_SIM_UART_OVERSAMPLING                RADIO_SIM_UART_OVERSAMPLING

// Initialization modes
#define UART_RADIO_ONLY_SET_TX_PIN_AS_PERIPHERAL        0
#define UART_RADIO_NORMAL_INITIALIZATION                1

#endif // UART_RADIO_SIM_CONFIG_H_

//! \} End of uart_radio_sim group
