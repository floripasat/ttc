/*
 * radio_hal_config.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-TTC.
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
 * \brief Radio HAL configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.8
 * 
 * \date 09/11/2017
 * 
 * \defgroup radio_hal_config Config
 * \ingroup radio_hal
 * \{
 */

#ifndef RADIO_HAL_CONFIG_H_
#define RADIO_HAL_CONFIG_H_

#include <config/config.h>

#define RADIO_HAL_MODULE_NAME               "Radio"

#define RADIO_HAL_RX_ISR_PORT_VECTOR        RADIO_GPIO_nIRQ_ISR_VECTOR

#endif // RADIO_HAL_CONFIG_H_

//! \} End of radio_hal group
