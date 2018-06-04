/*
 * cpu.h
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
 * \file cpu.h
 * 
 * \brief CPU initialization functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/07/2017
 * 
 * \defgroup cpu CPU
 * \ingroup hal
 * \{
 */

#ifndef CPU_H
#define CPU_H_

/**
 * \brief Configures the CPU clocks.
 * 
 * Clocks values:
 *      - MCLK = 16 MHz
 *      - SMCLK = 4 MHz
 *      - ACLK = 32,768 kHz
 *      .
 * 
 * \return None.
 */
void cpu_init();

#endif // CPU_H_

//! \} End of cpu group
