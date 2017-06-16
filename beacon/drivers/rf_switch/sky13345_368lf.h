/*
 * sky13345_368lf.h
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
 * \file sky13345_368lf.h
 * 
 * \brief SKY13345-368LF driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 11/06/2017
 * 
 * \defgroup sky13345_368lf SKY13345-368LF
 * \ingroup drivers
 * \{
 */

#ifndef SKY13345_368LF_H_
#define SKY13345_368LF_H_

/**
 * \fn sky13345_368lf_init
 * 
 * \brief SKY13345-368LF initialization.
 * 
 * \return None
 */
void sky13345_368lf_init();

/**
 * \fn sky13345_368lf_enable
 * 
 * \brief Enables beacon position on SKY13345-368LF.
 * 
 * \return None
 */
void sky13345_368lf_enable();

/**
 * \fn sky13345_368lf
 * 
 * \brief Disables beacon position on SKY13345-368LF.
 * 
 * \return None
 */
void sky13345_368lf_disable();

#endif // SKY13345_368LF_H_

//! \} End of sky13345_368lf group
