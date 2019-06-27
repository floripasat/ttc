/*
 * telecommands.h
 * 
 * Copyright (C) 2019, Universidade Federal de Santa Catarina.
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
 * \brief Telecommands definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.8
 * 
 * \date 30/05/2019
 * 
 * \defgroup telecommands Telecommands
 * \ingroup config
 * \{
 */

#ifndef TELECOMMANDS_H_
#define TELECOMMANDS_H_

#define TELECOMMAND_ID_PING_REQUEST         0x30
#define TELECOMMAND_ID_ENTER_HIBERNATION    0x31
#define TELECOMMAND_ID_LEAVE_HIBERNATION    0x32
#define TELECOMMAND_ID_BROADCAST_MESSAGE    0x33

#endif // TELECOMMANDS_H_

//! \} End of telecommands group
