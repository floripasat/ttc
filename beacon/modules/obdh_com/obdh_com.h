/*
 * gpio-obdh.h
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
 * \file obdh_com.h
 * 
 * \brief OBDH communication routines.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \defgroup obdh_com OBDH Communication
 * \ingroup modules
 * \{
 */

#ifndef OBDH_COM_H_
#define OBDH_COM_H_

#include <stdint.h>

/**
 * \fn obdh_com_init
 * 
 * \brief Initialization of the OBDH communication.
 * 
 * \return None
 */
void obdh_com_init();

/**
 * \fn obdh_com_send_cmd
 * 
 * \brief Sends a command to the OBDH module.
 * 
 * \param cmd is the command to send to the OBDH module.
 * 
 * \return None
 */
void obdh_com_send_cmd(uint8_t cmd);

/**
 * \fn obdh_com_spi_init
 * 
 * \brief OBDH communication SPI initialization.
 * 
 * \return Initialization status. It can be:
 *              -\b STATUS_SUCCESS
 *              -\b STATUS_FAIL
 *              .
 */
static uint8_t obdh_com_spi_init();

/**
 * \fn obdh_com_receive_data
 * 
 * \brief Receives data from the OBDH interruption.
 * 
 * \return None
 */
static void obdh_com_receive_data();

#endif // OBDH_COM_H_

//! \} End of obdh_com group
