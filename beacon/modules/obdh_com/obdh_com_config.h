/*
 * gpio_obdh_config.h
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
 * \file obdh_com_config.h
 * 
 * \brief OBDH communication configuration parameters.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/07/2017
 * 
 * \addtogroup obdh_com
 * \{
 */

#ifndef OBDH_COM_CONFIG_H_
#define OBDH_COM_CONFIG_H_

#define OBDH_COM_CMD_POSITION               0
#define OBDH_COM_CMD_START_OF_DATA_TRANSFER 0x80
#define OBDH_COM_CMD_SHUTDOWN_REQUEST       0x40
#define OBDH_COM_CMD_RF_MUTEX               0x20

#define OBDH_COM_SHUTDOWN_ACK               0x01
#define OBDH_COM_BEACON_FREE                0x10
#define OBDH_COM_BEACON_BUSY                0x11

#define OBDH_COM_TRANSCEIVER_FREE           0x10

#define OBDH_COM_START_OF_DATA              0x7E
#define OBDH_COM_SOD_POSITION               (OBDH_COM_CMD_POSITION + 1)

#define OBDH_COM_DATA_PKT_LEN               50

#define OBDH_COM_CRC_INITIAL_VALUE          0x03
#define OBDH_COM_CRC_POLYNOMIAL             0x92    // (BIT7 | BIT4 | BIT1) or x^7 + x^4 + x
#define OBDH_COM_CRC_POSITION               (OBDH_COM_SOD_POSITION + OBDH_COM_DATA_PKT_LEN + 1)

#define OBDH_COM_VBAT1_LEN                  2
#define OBDH_COM_VBAT1_POS                  (OBDH_COM_SOD_POSITION + 1)

#define OBDH_COM_VBAT2_LEN                  2
#define OBDH_COM_VBAT2_POS                  (OBDH_COM_VBAT1_POS + OBDH_COM_VBAT2_LEN)

#define OBDH_COM_I_SOLAR_PANELS_LEN         12
#define OBDH_COM_I_SOLAR_PANELS_POS         (OBDH_COM_VBAT2_POS + OBDH_COM_I_SOLAR_PANELS_LEN)

#define OBDH_COM_V_SOLAR_PANELS_LEN         6
#define OBDH_COM_V_SOLAR_PANELS_POS         (OBDH_COM_I_SOLAR_PANELS_POS + OBDH_COM_V_SOLAR_PANELS_LEN)

#define OBDH_COM_TEMP_BATTS_LEN             6
#define OBDH_COM_TEMP_BATTS_POS             (OBDH_COM_V_SOLAR_PANELS_POS + OBDH_COM_TEMP_BATTS_LEN)

#define OBDH_COM_IMU_LEN                    12
#define OBDH_COM_IMU_POS                    (OBDH_COM_TEMP_BATTS_POS + OBDH_COM_IMU_LEN)

#define OBDH_COM_Q_BATTS_LEN                2
#define OBDH_COM_Q_BATTS_POS                (OBDH_COM_IMU_POS + OBDH_COM_Q_BATTS_LEN)

#define OBDH_COM_SYSTEM_TIME_LEN            4
#define OBDH_COM_SYSTEM_TIME_POS            (OBDH_COM_Q_BATTS_POS + OBDH_COM_SYSTEM_TIME_LEN)

#define OBDH_COM_SAT_STATUS_LEN             2
#define OBDH_COM_SAT_STATUS_POS             (OBDH_COM_SYSTEM_TIME_POS + OBDH_COM_FSAT_STATUS_LEN)

#define OBDH_COM_RESET_COUNTER_LEN          2
#define OBDH_COM_RESET_COUNTER_POS          (OBDH_COM_SAT_STATUS_POS + OBDH_COM_RESET_COUNTER_LEN)

#endif // OBDH_COM_CONFIG_H_

//! \} End of obdh_com group
