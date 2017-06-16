/*
 * radio.h
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
 * \file radio.h
 * 
 * \brief Radio module header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \defgroup radio Radio
 * \ingroup modules
 * \{
 */

#ifndef RADIO_H_
#deifne RADIO_H_

#include <stdint.h>
#include <config/config.h>

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    #include <drivers/radio/cc11x5/cc11xx.h>
#elif BEACON_RADIO == SI4063
    #include <drivers/radio/si446x/si446x.h>
#elif BEACON_RADIO == RFM23BPW
    #include <drivers/radio/rfm23bpw.h>
#elif BEACON_RADIO == RF4463F30
    #include <drivers/radio/rf4463f30.h>
#elif BEACON_RADIO == UART_SIM
    #include <drivers/radio/uart_radio.h>
#endif // BEACON_RADIO

/**
 * \fn radio_init
 * 
 * \brief Initialization of the radio module.
 * 
 * \return 
 */
uint8_t radio_init();

/**
 * \fn radio_reset
 * 
 * \brief Resets the radio module.
 * 
 * \return None
 */
void radio_reset();

/**
 * \fn radio_write_data
 * 
 * \brief Write data to the radio module.
 * 
 * \return None
 */
void radio_write_data();

/**
 * \fn radio_read_data
 * 
 * \brief Read data from the radio module.
 * 
 * \return None
 */
void radio_read_data();

#endif // RADIO_H_

//! \} End of radio group
