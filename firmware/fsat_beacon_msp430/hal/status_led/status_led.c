/*
 * status_led.c
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
 * \brief Initialization and control of the status led (Implementation).
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.7
 * 
 * \date 11/10/2016
 * \addtogroup status_led
 * \{
 */

#include <drivers/driverlib/driverlib.h>

#include "status_led.h"

void status_led_init()
{
    GPIO_setAsOutputPin(STATUS_LED_PORT, STATUS_LED_PIN);
    
    status_led_disable();
}

void status_led_enable()
{
    GPIO_setOutputHighOnPin(STATUS_LED_PORT, STATUS_LED_PIN);
}

void status_led_disable()
{
    GPIO_setOutputLowOnPin(STATUS_LED_PORT, STATUS_LED_PIN);
}

void status_led_toggle()
{
    GPIO_toggleOutputOnPin(STATUS_LED_PORT, STATUS_LED_PIN);
}

//! \} End of status_led group
