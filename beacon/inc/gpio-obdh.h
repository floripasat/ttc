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

#include "../driverlib/driverlib.h"

#define OBDH_GPIO_PORT  GPIO_PORT_P4
#define OBDH_GPIO_PIN0  GPIO_PIN7
#define OBDH_GPIO_PIN1  GPIO_PIN6
#define OBDH_GPIO_PIN2  GPIO_PIN5
#define OBDH_GPIO_PIN3  GPIO_PIN4

#define OBDH_GPIO_STATE_WAITING_BIT0    0x00
#define OBDH_GPIO_STATE_WAITING_BIT1    0x01
#define OBDH_GPIO_STATE_WAITING_BIT2    0x02
#define OBDH_GPIO_STATE_WAITING_BIT3    0x03
#define OBDH_GPIO_STATE_ERROR           0x04

#define OBDH_GPIO_MAX_TRANSMISSION_TIME_SEC 2

extern uint8_t obdh_gpio_state;
extern uint8_t beacon_24_hours_sleep_mode;

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
 * \fn obdh_GPIO_Timer_Init
 * 
 * \brief Initialization of a timer to establish a time limit of transmission between OBDH and BEACON.
 * 
 * \return None
 */
void obdh_GPIO_Timer_Init();

#endif // GPIO_OBDH_H_

//! \} End of gpio-obdh group
