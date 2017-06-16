/*
 * si446x_pinmap.h
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
 * \file si446x_pinmap.h
 * 
 * \brief Silicon Labs SI446x driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 03/06/2017
 * 
 * \defgroup si446x_pinmap Pinmap
 * \ingroup si446x
 * \{
 */

#ifndef SI446X_PINMAP_H_
#define SI446X_PINMAP_H_

#include <config/config.h>

#define SI446X_SPI_USCI                 RADIO_SPI_USCI
#define SI446X_SPI_BASE_ADDRESS         RADIO_SPI_BASE_ADDRESS
#define SI446X_SPI_PORT                 RADIO_SPI_PORT

#define SI446X_nSEL_PORT                RADIO_SPI_NSEL_PORT
#define SI446X_nSEL_PIN                 RADIO_SPI_NSEL_PIN

#define SI446X_SDI_PORT                 RADIO_SPI_MOSI_PORT
#define SI446X_SDI_PIN                  RADIO_SPI_MOSI_PIN

#define SI446X_SDO_PORT                 RADIO_SPI_MISO_PORT
#define SI446X_SDO_PIN                  RADIO_SPI_MISO_PIN

#define SI446X_SCLK_PORT                RADIO_SPI_SCLK_PORT
#define SI446X_SCLK_PIN                 RADIO_SPI_SCLK_PIN

#define SI446X_SDN_PORT                 RADIO_GPIO_SDN_PORT
#define SI446X_SDN_PIN                  RADIO_GPIO_SDN_PIN

#define SI446X_nIRQ_PORT                RADIO_GPIO_nIRQ_PORT
#define SI446X_nIRQ_PIN                 RADIO_GPIO_nIRQ_PIN

#define SI446X_GPIO0_PORT               RADIO_GPIO_0_PORT
#define SI446X_GPIO0_PIN                RADIO_GPIO_0_PIN

#define SI446X_GPIO1_PORT               RADIO_GPIO_1_PORT
#define SI446X_GPIO1_PIN                RADIO_GPIO_1_PIN

#endif // SI446X_PINMAP_H_

//! \} End of si446x_pinmap group
