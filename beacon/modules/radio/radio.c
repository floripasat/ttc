/*
 * radio.c
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
 * \file radio.c
 * 
 * \brief Radio module implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \addtogroup radio
 * \{
 */

#include "radio.h"

uint8_t radio_init()
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    return cc11xx_init();
#elif BEACON_RADIO == SI4063
    return si406x_init();
#elif BEACON_RADIO == RFM23BPW
    return rfm23bpw_init();
#elif BEACON_RADIO == RF4463F30
    return rf4463f30_init();
#elif BEACON_RADIO == UART_SIM
    return uart_sim_init();
#endif // BEACON_RADIO
}

void radio_reset()
{
    
}

void radio_write_data()
{
    
}

void radio_read_data()
{
    
}

//! \} End of radio group
