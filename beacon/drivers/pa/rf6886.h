/*
 * rf6886.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina.
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
 * \brief Gain control of the RF6886 PA.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup rf6886 RF6886
 * \ingroup drivers
 * \{
 */

#ifndef RF6886_H_
#define RF6886_H_

#include <stdint.h>

/**
 * \brief Initialization of the power amplifier of the beacon (RF6886)
 * 
 * This function settings the MCU DAC12, and initizalizes the ENE_3V3_PA_BEACON pin.
 * The ENE_3V3_PA_BEACON pin tells to the EPS module to enable the power regulator of the PA.
 * 
 * 
 * After the DAC12 configuration, the operation sequence of the RF6886, is as follow.
 * 
 * RF6886 Turn on/off sequence (RF6886 datasheet, pg. 6):
 *      - Turn on:
 *          -# Apply Vcc
 *          -# Apply Vref1/2
 *          -# Apply drive at RF input
 *          .
 *      - Turn off:
 *          -# Remove drive at RF input
 *          -# Bring down voltage at Vref1/2
 *          -# Bring down Vcc (not necessary in system of course)
 *          .
 *      .
 * 
 * \return Initialization status. It can be:
 *      - STATUS_SUCCESS
 *      - STATUS_FAIL
 *      .
 */
uint8_t rf6886_init();

/**
 * \brief Enables the RF6886.
 * 
 * This function enable the ENE_3V3_PA_BEACON pin (turn on the PA).
 * Also, it enables the MCU DAC12 output, with or without (0, in this case)
 * a previous output voltage setting.
 * 
 * \return None.
 */
void rf6886_enable();

/**
 * \brief Disables the RF6886
 * 
 * This function disables the MCU DAC12 output (bringing it to zero), and the PA
 * power regulator (ENE_3V3_PA_BEACON).
 * 
 * \return None.
 */
void rf6886_disable();

/**
 * \brief RF6886 Vreg1/2 voltage.
 * 
 * This function sets the DAC output voltage.
 * It converts an input voltage (input of the function)
 * to a 12 bit integer, using a reference voltage (Vcc in this case).
 * V_ref = 0xFFF
 * -----   -----    --->  x = 0xFFF*V_reg/V_ref
 * V_reg     x
 * 
 * \note
 * Example:
 * V_ref = 3,3V
 * V_reg = 1,5V
 * x = 0x745
 * 
 * \param v_reg is the DAC output voltage,
 * 
 * \return None.
 */
void rf6886_set_v_reg(float v_reg);

#endif // RF6886_H_

//! \} End of rf6886 group
