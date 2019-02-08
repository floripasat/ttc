/*
 * hardware.h
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
 * \brief Hardware definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/06/2017
 * 
 * \defgroup hardware Hardware definition
 * \ingroup config
 * \{
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

//#######################################
//-- COMMON -----------------------------
//#######################################

#define HW_NONE             0

//#######################################
//-- RADIOS -----------------------------
//#######################################

#define CC1175              1
#define CC1125              2
#define SI4463              3
#define RF4463F30           4
#define UART_SIM            5

//#######################################
//-- PA ---------------------------------
//#######################################

#define RF6886              6

//#######################################
//-- RF SWITCH --------------------------
//#######################################

#define SKY13345_368LF      7

//#######################################
//-- ANTENNA ----------------------------
//#######################################

#define ISIS_ANTENNA        8
#define PASSIVE_ANTENNA     9

#endif // HARDWARE_H_

//! \} End of hardware group
