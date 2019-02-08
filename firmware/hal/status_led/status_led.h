/*
 * status_led.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina
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
 * \file status_led.h
 * 
 * \brief Initialization and control of the status led.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 11/10/2016
 * 
 * \defgroup status_led Status LED
 * \ingroup hal
 * \{
 */

#ifndef STATUS_LED_H_
#define STATUS_LED_H_

#include <config/config.h>

#define STATUS_LED_PORT         STATUS_LED_GPIO_PORT
#define STATUS_LED_PIN          STATUS_LED_GPIO_PIN

/**
 * \brief Initialization of the status led.
 * 
 * \return None.
 */
void status_led_init();

/**
 * \brief Turns on the status led.
 * 
 * \return None.
 */
void status_led_enable();

/**
 * \brief Turns off the status led.
 * 
 * \return None.
 */
void status_led_disable();

/**
 * \brief Toggles the status led.
 * 
 * \return None.
 */
void status_led_toggle();

#endif // STATUS_LED_H_

//! \} End of status_led group
