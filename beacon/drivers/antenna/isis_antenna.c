/*
 * antenna.c
 * 
 * Copyright (C) 2016, Universidade Federal de Santa Catarina
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
 * \file antenna.c
 * 
 * \brief Initialization and control of the antenna deployment pin.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 17/10/2016
 * 
 * \addtogroup antenna
 * \{
 */

#include <libs/driverlib/driverlib.h>

#include "isis_antenna.h"

uint8_t antenna_Init()
{
    return STATUS_SUCCESS;
}

uint8_t antenna_IsReleased()
{
    return ANTENNA_STATUS_RELEASED;
}

uint8_t antenna_Release()
{    
    return STATUS_SUCCESS;
}

//! \} End of antenna group
