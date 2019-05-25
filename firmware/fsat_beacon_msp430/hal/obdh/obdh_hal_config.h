/*
 * obdh_hal_config.h
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
 * \brief OBDH HAL configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.4
 * 
 * \date 08/07/2017
 * 
 * \defgroup obdh_hal_config Configuration
 * \ingroup obdh_hal
 * \{
 */

#ifndef OBDH_HAL_CONFIG_H_
#define OBDH_HAL_CONFIG_H_

#define OBDH_COM_MODULE_NAME                "OBDH"

// Timeout timer base address
#define OBDH_COM_TIMEOUT_TIMER_BASE         TIMER_B0_BASE

// Default byte of the data
#define OBDH_COM_DEFAULT_DATA_BYTE          0xFF

// OBDH->Beacon Commands
#define OBDH_COM_CMD_POSITION               0
#define OBDH_COM_CMD_START_OF_DATA_TRANSFER 0x80
#define OBDH_COM_CMD_HIBERNATION_REQUEST    0x40
#define OBDH_COM_CMD_RF_MUTEX               0x20

// Beacon->OBDH Commands
#define OBDH_COM_HIBERNATION_ACK            0x01
#define OBDH_COM_BEACON_FREE                0x10
#define OBDH_COM_BEACON_BUSY                0x11

// OBDH->Beacon Acknowledge
#define OBDH_COM_TRANSCEIVER_FREE           0x10

// Start Of Data
#define OBDH_COM_START_OF_DATA              0x7E
#define OBDH_COM_SOD_POSITION               (OBDH_COM_CMD_POSITION + 1)

// Data Packet Length
#define OBDH_COM_DATA_PKT_LEN               50

// CRC
#define OBDH_COM_CRC_INITIAL_VALUE          0x03
#define OBDH_COM_CRC_POLYNOMIAL             0x92    // (BIT7 | BIT4 | BIT1) or x^7 + x^4 + x
#define OBDH_COM_CRC_POSITION               (OBDH_COM_SOD_POSITION + OBDH_COM_DATA_PKT_LEN + 1)

// Battery 1 Voltage
#define OBDH_COM_BAT1_VOLTAGE_LEN           2
#define OBDH_COM_BAT1_VOLTAGE_POS           (OBDH_COM_SOD_POSITION + 1)

// Battery 2 Voltage
#define OBDH_COM_BAT2_VOLTAGE_LEN           2
#define OBDH_COM_BAT2_VOLTAGE_POS           (OBDH_COM_BAT1_VOLTAGE_POS + OBDH_COM_BAT1_VOLTAGE_LEN)

// Battery 1 Temperature
#define OBDH_COM_BAT1_TEMPERATURE_LEN       3
#define OBDH_COM_BAT1_TEMPERATURE_POS       (OBDH_COM_BAT2_VOLTAGE_POS + OBDH_COM_BAT2_VOLTAGE_LEN)

// Battery 2 Temperature
#define OBDH_COM_BAT2_TEMPERATURE_LEN       3
#define OBDH_COM_BAT2_TEMPERATURE_POS       (OBDH_COM_BAT1_TEMPERATURE_POS + OBDH_COM_BAT1_TEMPERATURE_LEN)

// Total Charge Of Batteries
#define OBDH_COM_BAT_CHARGE_LEN             2
#define OBDH_COM_BAT_CHARGE_POS             (OBDH_COM_BAT2_TEMPERATURE_POS + OBDH_COM_BAT2_TEMPERATURE_LEN)

// Solar Panels Currents
#define OBDH_COM_SOLAR_PANELS_CURRENTS_LEN  12
#define OBDH_COM_SOLAR_PANELS_CURRENTS_POS  (OBDH_COM_BAT_CHARGE_POS + OBDH_COM_BAT_CHARGE_LEN)

// Solar Panels Voltages
#define OBDH_COM_SOLAR_PANELS_VOLTAGES_LEN  6
#define OBDH_COM_SOLAR_PANELS_VOLTAGES_POS  (OBDH_COM_SOLAR_PANELS_CURRENTS_POS + OBDH_COM_SOLAR_PANELS_CURRENTS_LEN)

// Satellite Status
#define OBDH_COM_SAT_STATUS_LEN             2
#define OBDH_COM_SAT_STATUS_POS             (OBDH_COM_SOLAR_PANELS_VOLTAGES_POS + OBDH_COM_SOLAR_PANELS_VOLTAGES_LEN)

// IMU Data
#define OBDH_COM_IMU_LEN                    12
#define OBDH_COM_IMU_POS                    (OBDH_COM_SAT_STATUS_POS + OBDH_COM_SAT_STATUS_LEN)

// System Time
#define OBDH_COM_SYSTEM_TIME_LEN            4
#define OBDH_COM_SYSTEM_TIME_POS            (OBDH_COM_IMU_POS + OBDH_COM_IMU_LEN)

// OBDH Reset Counter
#define OBDH_COM_RESET_COUNTER_LEN          2
#define OBDH_COM_RESET_COUNTER_POS          (OBDH_COM_SYSTEM_TIME_POS + OBDH_COM_SYSTEM_TIME_LEN)

#endif // OBDH_HAL_CONFIG_H_

//! \} End of obdh_hal group
