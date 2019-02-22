/*
 * rf4463_pinmap.h
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
 * \brief NiceRF RF4463 pinmap.
 * 
 * This library suits for RF4463PRO and RF4463F30 in FIFO mode.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 16/06/2017
 * 
 * \addtogroup rf4463
 * \{
 */

#ifndef RF4463_PINMAP_H_
#define RF4463_PINMAP_H_

#include <config/config.h>

#define RF4463_SPI_PORT                 RADIO_SPI_PORT
#define RF4463_SPI_USCI                 RADIO_SPI_USCI
#define RF4463_SPI_BASE_ADDRESS         RADIO_SPI_BASE_ADDRESS

#define RF4463_NSEL_PORT                RADIO_SPI_NSEL_PORT
#define RF4463_NSEL_PIN                 RADIO_SPI_NSEL_PIN

#define RF4463_SDI_PORT                 RADIO_SPI_MOSI_PORT
#define RF4463_SDI_PIN                  RADIO_SPI_MOSI_PIN

#define RF4463_SDO_PORT                 RADIO_SPI_MISO_PORT
#define RF4463_SDO_PIN                  RADIO_SPI_MISO_PIN

#define RF4463_SCLK_PORT                RADIO_SPI_SCLK_PORT
#define RF4463_SCLK_PIN                 RADIO_SPI_SCLK_PIN

#define RF4463_SDN_PORT                 RADIO_GPIO_SDN_PORT
#define RF4463_SDN_PIN                  RADIO_GPIO_SDN_PIN

#define RF4463_nIRQ_PORT                RADIO_GPIO_nIRQ_PORT
#define RF4463_nIRQ_PIN                 RADIO_GPIO_nIRQ_PIN

#define RF4463_GPIO0_PORT               RADIO_GPIO_0_PORT
#define RF4463_GPIO0_PIN                RADIO_GPIO_0_PIN

#define RF4463_GPIO1_PORT               RADIO_GPIO_1_PORT
#define RF4463_GPIO1_PIN                RADIO_GPIO_1_PIN

#define RF4463_POWER_ENABLE_PORT        RADIO_POWER_ENABLE_PORT
#define RF4463_POWER_ENABLE_PIN         RADIO_POWER_ENABLE_PIN

#endif // RF4463_PINMAP_H_

//! \} End of rf4463 group
