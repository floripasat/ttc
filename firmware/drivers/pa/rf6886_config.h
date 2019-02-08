/*
 * rf6886_config.h
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
 * \brief Configuration of the RF6886 PA.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 11/06/2017
 * 
 * \defgroup rf6886_config RF6886 Configuration
 * \ingroup rf6886
 * \{
 */

#ifndef RF6886_CONFIG_H_
#define RF6886_CONFIG_H_

#include <config/config.h>

#define RF6886_V_REF                BEACON_PA_VREF

#define RF6886_DAC_BASE_ADDRESS     BEACON_PA_DAC_BASE_ADDRESS
#define RF6886_DAC_SUBMODULE        BEACON_PA_DAC_SUBMODULE

#endif // RF6886_CONFIG_H_

//! \} End of rf6886_config group
