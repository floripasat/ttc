/*
 * init.h
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
 * \file init.h
 * 
 * \brief Initialization routines header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/06/2017
 * 
 * \defgroup init Initialization
 * \{
 */

#ifndef INIT_H_
#define INIT_H_

#include <stdint.h>
#include <modules/eps_com/eps_com.h>
#include <modules/obdh_com/obdh_com.h>

#include "beacon.h"

/**
 * \fn init_debug
 * 
 * \brief Debug mode initialization routine.
 * 
 * \return None
 */
void init_debug();

/**
 * \fn init_watchdog
 * 
 * \brief Watchdog initialization routine.
 * 
 * \return None
 */
void init_watchdog();

/**
 * \fn init_cpu
 * 
 * \brief CPU initialization routine.
 * 
 * \return None
 */
void init_cpu();

/**
 * \fn init_memory
 * 
 * \brief Non-volatile memory initialization routine.
 * 
 * \return None
 */
void init_memory();

/**
 * \fn init_status_led
 * 
 * \brief Status LED initialization routine.
 * 
 * \return None
 */
void init_status_led();

/**
 * \fn init_time
 * 
 * \brief Time control initialization routine.
 * 
 * \param sec is a pointer to the second counter variable.
 * 
 * \return None
 */
void init_time(uint32_t *sec);

/**
 * \fn init_antenna
 * 
 * \brief Antenna control initialization routine.
 * 
 * \return None
 */
void init_antenna();

/**
 * \fn init_eps_com
 * 
 * \brief EPS communication initialization routine.
 * 
 * \param eps is a pointer to an eps struct (object).
 * \param system_time is a pointer to the system time counter.
 * 
 * \return None
 */
void init_eps_com(EPS *eps, uint32_t *system_time);

/**
 * \fn init_obdh_com
 * 
 * \brief OBDH communication initialization routine.
 * 
 * \param obdh is a pointer to an obdh struct (object).
 * \param system_time is a pointer to the system time counter.
 * 
 * \return None
 */
void init_obdh_com(OBDH *obdh, uint32_t *system_time);

/**
 * \fn init_radio
 * 
 * \brief Radio initialization routine.
 * 
 * \return None
 */
void init_radio();

/**
 * \fn init_rf_switch
 * 
 * \brief RF switch initialization routine.
 * 
 * \return None
 */
void init_rf_switch();

/**
 * \fn init_pa
 * 
 * \brief PA initialization routine.
 * 
 * \return None
 */
void init_pa();

/**
 * \fn init_protocols
 * 
 * \brief Communication protocols initialization routine.
 * 
 * \return None
 */
void init_protocols();

#endif // INIT_H_

//! \} End of init group
