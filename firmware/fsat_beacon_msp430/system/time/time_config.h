/*
 * time.h
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
 * \brief Time control configuration.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.4
 * 
 * \date 23/02/2019
 * 
 * \defgroup time_config Configuration
 * \ingroup time
 * \{
 */

#ifndef TIME_CONFIG_H_
#define TIME_CONFIG_H_

#define TIME_MODULE_NAME            "Time Control"

#define TIME_CRC8_INITIAL_VALUE     0x00        /**< CRC8-CCITT. */
#define TIME_CRC8_POLYNOMIAL        0x07        /**< CRC8-CCITT. */

#endif // TIME_CONFIG_H_

//! \} End of time_config group
