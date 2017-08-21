/*
 * config.h
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
 * \file config.h
 * 
 * \brief General configuration definitions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/06/2017
 * 
 * \defgroup config General configuration
 * \ingroup beacon
 * \{
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <libs/driverlib/driverlib.h>

#include "modes.h"
#include "pinmap.h"
#include "hardware.h"

//########################################################
//-- SATELLITE DATA --------------------------------------
//########################################################

#define SATELLITE_ID                        "FLORIPASAT"

#define SATELLITE_ENERGY_LEVEL_1            0
#define SATELLITE_ENERGY_LEVEL_2            1
#define SATELLITE_ENERGY_LEVEL_3            2
#define SATELLITE_ENERGY_LEVEL_4            3
#define SATELLITE_ENERGY_LEVEL_5            4

//########################################################
//-- BEACON OPERATION ------------------------------------
//########################################################

#define BEACON_MODE                         TEST_MODE

#define BEACON_TX_PERIOD_SEC_L1             10
#define BEACON_TX_PERIOD_SEC_L2             10
#define BEACON_TX_PERIOD_SEC_L3             20
#define BEACON_TX_PERIOD_SEC_L4             30
#define BEACON_TX_PERIOD_SEC_L5             30

#define BEACON_HIBERNATION_PERIOD_SECONDS   0
#define BEACON_HIBERNATION_PERIOD_MINUTES   0
#define BEACON_HIBERNATION_PERIOD_HOURS     24

#define BEACON_ANTENNA_DEPLOY_SLEEP_SEC     0
#define BEACON_ANTENNA_DEPLOY_SLEEP_MIN     45
#define BEACON_ANTENNA_DEPLOY_SLEEP_HOUR    0

//########################################################
//-- BEACON PACKETS --------------------------------------
//########################################################

#define BEACON_PACKET_PAYLOAD_CONTENT       (PAYLOAD_SAT_ID | PAYLOAD_OBDH_DATA | PAYLOAD_EPS_DATA)

#define BEACON_PACKET_PROTOCOL              (PACKET_NGHAM)

//########################################################
//-- RADIO -----------------------------------------------
//########################################################

#define BEACON_RADIO                        RF4463F30
#define BEACON_RADIO_SPI_CLK                100000

//########################################################
//-- PA --------------------------------------------------
//########################################################

#define BEACON_PA                           HW_NONE

#define BEACON_PA_VREF                      3.3

#define BEACON_PA_DAC_BASE_ADDRESS          DAC12_A_BASE
#define BEACON_PA_DAC_SUBMODULE             DAC12_A_SUBMODULE_0

//########################################################
//-- RF SWITCH -------------------------------------------
//########################################################

#define BEACON_RF_SWITCH                    HW_NONE

//########################################################
//-- ANTENNA ---------------------------------------------
//########################################################

#define BEACON_ANTENNA                      PASSIVE_ANTENNA
#define BEACON_ANTENNA_I2C_CLK              100000

//########################################################
//-- CPU -------------------------------------------------
//########################################################

#define BEACON_CPU_FREQ_HZ                  16000000
#define BEACON_CPU_FREQ_KHZ                 (BEACON_CPU_FREQ_HZ/1000)
#define BEACON_CPU_FREQ_MHZ                 (BEACON_CPU_FREQ_HZ/1000000)

//########################################################
//-- WATCHDOG --------------------------------------------
//########################################################

#define WATCHDOG_BASE_ADDRESS               WDT_A_BASE
#define WATCHDOG_CLK_SOURCE                 WDT_A_CLOCKSOURCE_ACLK
#define WATCHDOG_CLK_DIVIDER                WDT_A_CLOCKDIVIDER_512K  // 16 seconds WDT

//########################################################
//-- TIMER -----------------------------------------------
//########################################################

#define TIMER_BASE_ADDRESS                  TIMER_A1_BASE

//########################################################
//-- OBDH COMMUNICATION ----------------------------------
//########################################################

#define OBDH_COM_DEADLINE_SEC               60
#define OBDH_COM_DEADLINE_MIN               1

//########################################################
//-- EPS COMMUNICATION -----------------------------------
//########################################################

#define EPS_COM_DEADLINE_SEC                60
#define EPS_COM_DEADLINE_MIN                1

#endif // CONFIG_H_

//! \} End of config group
