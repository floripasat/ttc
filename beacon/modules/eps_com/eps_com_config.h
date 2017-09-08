/*
 * eps_com_config.h
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
 * \file eps_com_config.h
 * 
 * \brief EPS communication configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 13/07/2017
 * 
 * \addtogroup eps_com
 * \{
 */

#ifndef EPS_COM_CONFIG_H_
#define EPS_COM_CONFIG_H_

// Timeout timer base address
#define EPS_COM_TIMEOUT_TIMER_BASE          TIMER_A0_BASE

// Default byte of the data
#define EPS_COM_DEFAULT_DATA_BYTE           0xFF

// Start Of Data
#define EPS_COM_PKT_SOD                     0x7E
#define EPS_COM_PKT_SOD_POSITION            0

// Data Packet Length
#define EPS_COM_DATA_PKT_LEN                31

// CRC
#define EPS_COM_CRC_INITIAL_VALUE           0x03
#define EPS_COM_CRC_POLYNOMIAL              0x92
#define EPS_COM_CRC_POSITION                (EPS_COM_PKT_SOD_POSITION + EPS_COM_DATA_PKT_LEN + 1)

// Battery 1 Voltage
#define EPS_COM_BAT1_VOLTAGE_LEN            2
#define EPS_COM_BAT1_VOLTAGE_POS            (EPS_COM_PKT_SOD_POSITION + 1)

// Battery 2 Voltage
#define EPS_COM_BAT2_VOLTAGE_LEN            2
#define EPS_COM_BAT2_VOLTAGE_POS            (EPS_COM_BAT1_VOLTAGE_POS + EPS_COM_BAT1_VOLTAGE_LEN)

// Battery 1 Temperature
#define EPS_COM_BAT1_TEMPERATURE_LEN        3
#define EPS_COM_BAT1_TEMPERATURE_POS        (EPS_COM_BAT2_VOLTAGE_POS + EPS_COM_BAT2_VOLTAGE_LEN)

// Battery 2 Temperature
#define EPS_COM_BAT2_TEMPERATURE_LEN        3
#define EPS_COM_BAT2_TEMPERATURE_POS        (EPS_COM_BAT1_TEMPERATURE_POS + EPS_COM_BAT1_TEMPERATURE_LEN)

// Total Charge of Batteries
#define EPS_COM_BAT_CHARGE_LEN              2
#define EPS_COM_BAT_CHARGE_POS              (EPS_COM_BAT2_TEMPERATURE_POS + EPS_COM_BAT2_TEMPERATURE_LEN)

// Solar Panels Currents
#define EPS_COM_SOLAR_PANELS_CURRENTS_LEN   12
#define EPS_COM_SOLAR_PANELS_CURRENTS_POS   (EPS_COM_BAT_CHARGE_POS + EPS_COM_BAT_CHARGE_LEN)

// Solar Panels Voltages
#define EPS_COM_SOLAR_PANELS_VOLTAGES_LEN   6
#define EPS_COM_SOLAR_PANELS_VOLTAGES_POS   (EPS_COM_SOLAR_PANELS_CURRENTS_POS + EPS_COM_SOLAR_PANELS_CURRENTS_LEN)

// Satellite Energy Level
#define EPS_COM_ENERGY_LEVEL_LEN            1
#define EPS_COM_ENERGY_LEVEL_POS            (EPS_COM_SOLAR_PANELS_VOLTAGES_POS + EPS_COM_SOLAR_PANELS_VOLTAGES_LEN)

#endif // EPS_COM_CONFIG_H_

//! \} End of eps_com group
