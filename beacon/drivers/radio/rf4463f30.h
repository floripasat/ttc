/*
 * rf4463f30.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
 
/**
 * \file rf4463f30.h
 * 
 * \brief NiceRF RF4463F30 driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 01/06/2017
 * 
 * \defgroup rf4463f30 RF4463F30
 * \ingroup beacon
 * \{
 */

#ifndef RF4463F30_H_
#define RF4463F30_H_

#include <stdint.h>

#include "drivers/si446x/pinmap.h"
#include "../driverlib/driverlib.h"

#ifndef USCI_A
#define USCI_A                          0x00
#endif // USCI_A

#ifndef USCI_B
#define USCI_B                          0x01
#endif // USCI_B

#define RF4463F30_SPI_CLK               100000          /**< SPI clock frequency in Hz. */

/**
 * \defgroup pins RF4463F30 pins
 * \ingroup rf4463f30
 * 
 * \brief MCU pins on RF4463F30.
 * 
 * \{
 */
#define RF4463F30_SPI_USCI              SI446X_SPI_USCI         /**< USCI interface */
#define RF4463F30_SPI_BASE_ADDRESS      SI446X_SPI_BASE_ADDRESS /**< Base address = USCI_B0_BASE */
#define RF4463F30_SPI_PORT              SI446X_SPI_PORT         /**< SPI port = P2 (all pins at the same port) */

#define RF4463F30_nSEL_PORT             SI446X_nSEL_PORT        /**< nSEL port = P2 */
#define RF4463F30_nSEL_PIN              SI446X_nSEL_PIN         /**< nSEL pin = P2.0 */

#define RF4463F30_SDI_PORT              SI446X_SDI_PORT         /**< SDI (MOSI) port = P2 */
#define RF4463F30_SDI_PIN               SI446X_SDI_PIN          /**< SDI (MOSI) pin = P2.1 */

#define RF4463F30_SDO_PORT              SI446X_SDO_PORT         /**< SDO (MISO) port = P2 */
#define RF4463F30_SDO_PIN               SI446X_SDO_PIN          /**< SDO (MISO) pin = P2.2 */

#define RF4463F30_SCLK_PORT             SI446X_SCLK_PORT        /**< SCLK port = P2 */
#define RF4463F30_SCLK_PIN              SI446X_SCLK_PIN         /**< SCLK pin = P2.3 */

#define RF4463F30_SDN_PORT              SI446X_SDN_PORT         /**< SDN port = P2 */
#define RF4463F30_SDN_PIN               SI446X_SDN_PIN          /**< SDN pin = P2.4 */

#define RF4463F30_nIRQ_PORT             SI446X_nIRQ_PORT        /**< nIRQ port = P2 */
#define RF4463F30_nIRQ_PIN              SI446X_nIRQ_PIN         /**< nIRQ pin = P2.4 */

#define RF4463F30_GPIO0_PORT            SI446X_GPIO0_PORT       /**< GPIO0 port = P1 */
#define RF4463F30_GPIO0_PIN             SI446X_GPIO0_PIN        /**< GPIO0 pin = P1.6 */

#define RF4463F30_GPIO1_PORT            SI446X_GPIO1_PORT       /**< GPIO1 port = P1 */
#define RF4463F30_GPIO1_PIN             SI446X_GPIO1_PIN        /**< GPIO1 pin = P1.5 */
//! \} End of pins


#define rf4463f30_init()                si446x_init()

#endif // RF4463F30_H_

//! \} End of rf4463f30 group
