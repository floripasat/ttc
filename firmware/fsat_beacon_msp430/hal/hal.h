/*
 * hal.h
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
 * \file hal.h
 * 
 * \brief HAL include file.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/11/2017
 * 
 * \defgroup hal HAL
 * \{
 */

#ifndef HAL_H_
#define HAL_H_

#include "antenna/antenna.h"
#include "eps/eps_hal.h"
#include "mcu/cpu.h"
#include "mcu/flash.h"
#include "mcu/watchdog.h"
#include "obdh/obdh_hal.h"
#include "pa/pa.h"
#include "radio/radio_hal.h"
#include "rf_switch/rf_switch.h"
#include "status_led/status_led.h"

#endif // HAL_H_

//! \} End of hal group
