/*
 * modules.h
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
 * \file modules.h
 * 
 * \brief Modules include file.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \defgroup modules Modules
 * \ingroup beacon
 * \{
 */

#ifndef MODULES_H_
#define MODULES_H_

#include "antenna/antenna.h"
#include "debug/debug.h"
#include "eps_com/eps_com.h"
#include "mcu/cpu.h"
#include "mcu/flash.h"
#include "mcu/watchdog.h"
#include "obdh_com/obdh_com.h"
#include "pa/pa.h"
#include "radio/radio.h"
#include "rf_switch/rf_switch.h"
#include "status_led/status_led.h"
#include "time/delay.h"
#include "time/time.h"

#endif // MODULES_H_

//! \} End of modules group
