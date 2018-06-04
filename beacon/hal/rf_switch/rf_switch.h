/*
 * rf_switch.h
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
 * \file rf_switch.h
 * 
 * \brief Control of the RF switch functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/03/2017
 * 
 * \defgroup rf_switch RF Switch
 * \ingroup hal
 * \{
 */

#ifndef RF_SWITCH_H_
#define RF_SWITCH_H_

/**
 * \brief Initilization routine of the RF switch.h
 * 
 * \return None.
 */
void rf_switch_init();

/**
 * \brief Puts the RF switch in beacon position.
 * 
 * \return None.
 */
void rf_switch_enable_beacon();

/**
 * \brief Puts the RF switch out of beacon position.
 * 
 * \return None.
 */
void rf_switch_disable_beacon();

#endif // RF_SWITCH_H_

//! \} End of rf_switch group
