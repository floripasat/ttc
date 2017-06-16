/*
 * flags.h
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
 * \file flags.h
 * 
 * \brief General operation flags.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/06/2017
 * 
 * \defgroup flags Flags
 * \ingroup src
 * \{
 */

#ifndef FLAGS_H_
#define FLAGS_H_

#include <stdint.h>

/**
 * \struct Flags
 * 
 * \brief Beacon general flags struct.
 */
struct Flags
{
    bool hibernation;
    bool can_transmit;
};

/**
 * \var flags
 * 
 * \brief Beacon general flags.
 */
extern Flags flags;

#endif // FLAGS_H_

//! \} End of flags group
