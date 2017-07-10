/*
 * beacon.h
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
 * \file beacon.h
 * 
 * \brief Beacon header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/06/2017
 * 
 * \defgroup beacon Beacon
 * \{
 */

#ifndef BEACON_H_
#define BEACON_H_

#include <modules/modules.h>

#include "flags.h"

/**
 * \struct Beacon
 * 
 * \brief Beacon variables struct.
 */
typedef struct
{
    Time hibernation_mode_initial_time;
    Flags flags;
} Beacon;

/**
 * \var beacon
 * 
 * \brief Beacon variables struct.
 */
extern Beacon beacon;

/**
 * \fn beacon_init
 * 
 * \brief Beacon initialization routine.
 * 
 * \return None
 */
void beacon_init();

/**
 * \fn beacon_run
 * 
 * \brief Main operation routine.
 * 
 * \return None
 */
void beacon_run();

/**
 * \fn beacon_deinit
 * 
 * \brief Beacon deinitialization routine.
 * 
 * \return None
 */
void beacon_deinit();

/**
 * \fn beacon_shutdown
 * 
 * \brief Shutdowns the system.
 * 
 * \return None
 */
void beacon_shutdown();

#endif // BEACON_H_

//! \} End of beacon group
