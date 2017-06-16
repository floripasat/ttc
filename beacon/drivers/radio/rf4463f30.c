/*
 * rf4463f30.c
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
 
/**
 * \file rf4463f30.c
 * 
 * \brief NiceRF RF4463F30 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 01/06/2017
 * 
 * \addtogroup rf4463f30
 * \{
 */

#include "../inc/si4463.h"

uint8_t rf4463f30_Init()
{
    GPIO_setAsOutputPin(RF4463F30_nSEL_PORT, RF4463F30_nSEL_PIN);
    GPIO_setAsOutputPin(RF4463F30_SDN_PORT, RF4463F30_SDN_PIN);
    GPIO_setAsOutputPin(RF4463F30_nIRQ_PORT, RF4463F30_nIRQ_PIN);
    GPIO_setAsOutputPin(RF4463F30_GPIO0_PORT, RF4463F30_GPIO0_PIN);
    GPIO_setAsOutputPin(RF4463F31_GPIO0_PORT, RF4463F30_GPIO1_PIN);
}

//! \} End of rf4463f30 implementation group
