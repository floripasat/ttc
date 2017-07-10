/*
 * uart_radio_sim.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
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
 * \file uart_radio_sim.h
 * 
 * \brief UART Radio Simulation driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 11/06/2017
 * 
 * \defgroup uart_radio_sim UART Radio Simulation
 * \ingroup drivers
 * \{
 */

#ifndef UART_RADIO_SIM_H_
#define UART_RADIO_SIM_H_

#include <stdint.h>

#include <config/config.h>

#define UART_RADIO_SIM_UART_USCI            RADIO_SIM_UART_USCI
#define UART_RADIO_SIM_UART_BASE_ADDRESS    RADIO_SIM_UART_BASE_ADDRESS
#define UART_RADIO_SIM_UART_TX_PORT         RADIO_SIM_UART_TX_PORT
#define UART_RADIO_SIM_UART_TX_PIN          RADIO_SIM_UART_TX_PIN

/**
 * \fn uart_radio_sim_init
 * 
 * \brief Initialization of the UART radio simulation.
 * 
 * \return Initiazaliton status. It can be:
 *              -\b STATUS_SUCCESS
 *              -\b STATUS_FAIL
 *              .
 */
uint8_t uart_radio_sim_init();

/**
 * \fn uart_radio_sim_send_data
 * 
 * \brief Send data through the UART port.
 * 
 * \param data is a pointer to the data to be written in the UART port.
 * \param size is the size of the data to be written in the UART port.
 * 
 * \return None
 */
void uart_radio_sim_send_data(uint8_t *data, uint16_t size);

#endif // UART_RADIO_SIM_H_

//! \} End of uart_radio_sim group
