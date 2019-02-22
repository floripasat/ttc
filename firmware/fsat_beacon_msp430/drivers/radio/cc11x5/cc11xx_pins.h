/*
 * cc11xx_pins.h
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
 * \brief CC1125/CC1175 pins map.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 10/06/2016
 * 
 * \defgroup cc11xx_pins Pins
 * \ingroup cc11xx
 * \{
 */

#ifndef CC11XX_PINS_H_
#define CC11XX_PINS_H_

#include <config/config.h>

#define CC11XX_RESET_PORT               RADIO_GPIO_nIRQ_PORT
#define CC11XX_RESET_PIN                RADIO_GPIO_nIRQ_PIN

#define CC11XX_SPI_USCI                 RADIO_SPI_USCI
#define CC11XX_SPI_BASE_ADDRESS         RADIO_SPI_BASE_ADDRESS
#define CC11XX_SPI_PORT                 RADIO_SPI_PORT

#define CC11XX_CSN_PORT                 RADIO_SPI_NSEL_PORT
#define CC11XX_CSN_PIN                  RADIO_SPI_NSEL_PIN

#define CC11XX_MOSI_PORT                RADIO_SPI_MOSI_PORT
#define CC11XX_MOSI_PIN                 RADIO_SPI_MOSI_PIN

#define CC11XX_MISO_PORT                RADIO_SPI_MISO_PORT
#define CC11XX_MISO_PIN                 RADIO_SPI_MISO_PIN

#define CC11XX_SCLK_PORT                RADIO_SPI_SCLK_PORT
#define CC11XX_SCLK_PIN                 RADIO_SPI_SCLK_PIN

#define CC11XX_GPIO0_PORT               RADIO_GPIO_0_PORT
#define CC11XX_GPIO0_PIN                RADIO_GPIO_0_PIN
#define CC11XX_GPIO2_PORT               RADIO_GPIO_1_PORT
#define CC11XX_GPIO2_PIN                RADIO_GPIO_1_PORT
#define CC11XX_GPIO3_PORT               RADIO_GPIO_SDN_PORT
#define CC11XX_GPIO3_PIN                RADIO_GPIO_SDN_PIN

#endif // CC11XX_PINS_H_

//! \} End of cc11xx group
