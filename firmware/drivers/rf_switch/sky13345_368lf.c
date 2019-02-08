/*
 * sky13345_368lf.c
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
 * \brief SKY13345-368LF driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 11/06/2017
 * 
 * \addtogroup sky13345_368lf
 * \{
 */

#include <drivers/driverlib/driverlib.h>

#include "sky13345_368lf.h"
#include "sky13345_368lf_pinmap.h"

void sky13345_368lf_init()
{
    GPIO_setAsOutputPin(SKY13345_368LF_GPIO_CTRL_PORT, SKY13345_368LF_GPIO_CTRL_PIN);
    
    sky13345_368lf_disable();
}

void sky13345_368lf_enable()
{
    GPIO_setOutputHighOnPin(SKY13345_368LF_GPIO_CTRL_PORT, SKY13345_368LF_GPIO_CTRL_PIN);
}

void sky13345_368lf_disable()
{
    GPIO_setOutputLowOnPin(SKY13345_368LF_GPIO_CTRL_PORT, SKY13345_368LF_GPIO_CTRL_PIN);
}

//! \} End of sky13345_368lf group
