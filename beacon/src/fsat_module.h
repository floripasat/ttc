/*
 * fsat_module.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
 * 
 * This file is part of FloripaSat-Beacon.
 * 
 * FloripaSat-Beacon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-Beacon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with FloripaSat-Beacon. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Generic FloripaSat module struct.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/11/2017
 * 
 * \defgroup fsat_module FloripaSat Module
 * \ingroup src
 * \{
 */

#ifndef FSAT_MODULE_H_
#define FSAT_MODULE_H_

#include <stdint.h>
#include <stdbool.h>

#include <system/buffer/buffer.h>

/**
 * \brief A struct to implement a generic module from the FloripaSat satellite.
 * 
 * This struct can be used only to modules with communication with the Beacon module.
 */
typedef struct
{
    uint32_t    time_last_valid_pkt;    /**< Time stamp of the last valid received packet. */
    uint8_t     errors;                 /**< Number of errors (Packets with errors). */
    bool        is_dead;                /**< If true, the module is not sending data, so it is possibly not working. */
    Buffer      buffer;                 /**< Last received data from the module. */
} FSatModule;

#endif // FSAT_MODULE_H_

//! \} End of fsat_module group
