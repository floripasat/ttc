/*
 * ax25_config.h
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
 * \brief AX25 protocol general configuration.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.2
 * 
 * \date 08/09/2017
 * 
 * \defgroup ax25_config Configuration
 * \ingroup ax25
 * \{
 */

#ifndef AX25_CONFIG_H_
#define AX25_CONFIG_H_

#include <config/config.h>

#define AX25_MODULE_NAME                "AX.25"                         /**< Module name in debug messages. */

#define AX25_FLORIPASAT_CALLSIGN        SATELLITE_CALLSIGN              /**< The FloripaSat project callsign. */
#define AX25_FLORIPASAT_GRS_SSID        0x00                            /**< The SSID of the groundstation. User defined field. */
#define AX25_FLORIPASAT_SAT_SSID        0x01                            /**< The SSID of the satellite. User defined field. */
#define AX25_FLORIPASAT_HEADER_SIZE     20                              /**< The size of the header. */

#define AX25_DESTINATION_CALLSIGN       BEACON_PACKET_DST_CALLSIGN      /**< Destination callsign. */

#endif // AX25_CONFIG_H_

//! \} End of ax25_config group
