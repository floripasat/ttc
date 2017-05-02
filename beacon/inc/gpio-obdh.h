/*
 * gpio-obdh.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \file gpio-obdh.h
 * 
 * \brief Functions to receive the 24 hours shutdown command from ODBH.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \defgroup gpio-obdh GPIO-OBDH
 * \ingroup beacon
 * \{
 */

#ifndef GPIO_OBDH_H_
#define GPIO_OBDH_H_

#include <stdint.h>

#include "../driverlib/driverlib.h"

/**
 * \brief OBDH-Beacon communication pin map.
 * 
 * \{
 */
#define OBDH_GPIO_PORT                  GPIO_PORT_P4
#define OBDH_GPIO_INTERRUPT_PIN         GPIO_PIN7
#define OBDH_GPIO_SHUTDOWN_PIN          GPIO_PIN6
#define OBDH_GPIO_TX_REQUEST_PIN        GPIO_PIN5
#define OBDH_GPIO_TX_BUSY_PIN           GPIO_PIN4
//! \}

/**
 * \brief Beacon to OBDH commands.
 * 
 * \{
 */
#define OBDH_GPIO_BEACON_TX_ON          0x00
#define OBDH_GPIO_BEACON_TX_OFF         0x01
//! \}

/**
 * \brief TX states of the OBDH module.
 * 
 * \{
 */
#define OBDH_GPIO_OBDH_TX_ON            0x00
#define OBDH_GPIO_OBDH_TX_OFF           0x01
//! \}

extern uint8_t obdh_gpio_obdh_state;

/**
 * \fn obdh_GPIO_Init
 * 
 * \brief Initialization of the OBDH GPIO communication.
 * 
 * This function initializes the OBDH_GPIO_PINS as input pins. The interruption
 * on OBDH_GPIO_PIN0 is also initialized with high to low transition.
 * 
 * \return None
 */
void obdh_GPIO_Init();

/**
 * \fn obdh_GPIO_SendToOBDH
 * 
 * \brief Send a message/command to OBDH module.
 * 
 * \param msg is the message to be sent.
 * 
 * \return None
 */
void obdh_GPIO_SendToOBDH(uint8_t msg);

#endif // GPIO_OBDH_H_

//! \} End of gpio-obdh group
