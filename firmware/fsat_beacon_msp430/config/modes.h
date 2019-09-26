/*
 * modes.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * \brief Operation modes definitions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.8
 * 
 * \date 08/06/2017
 * 
 * \defgroup modes Operation modes
 * \ingroup config
 * \{
 */

#ifndef MODES_H_
#define MODES_H_

#define PAYLOAD_SAT_ID                      1
#define PAYLOAD_OBDH_DATA                   2
#define PAYLOAD_EPS_DATA                    4

#define PACKET_NGHAM                        1
#define PACKET_AX25                         2
#define PACKET_RAW                          4

#define ANTENNA_INDEPENDENT_DEPLOY_MODE     1
#define ANTENNA_SEQUENTIAL_DEPLOY_MODE      2

#endif // MODES_H_

//! \} End of modes group
