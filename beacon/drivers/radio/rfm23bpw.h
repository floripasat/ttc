/*
 * rfm23bpw.h
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
 * \file rfm23bpw.h
 * 
 * \brief HopeRF RFM23BPW driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 01/06/2017
 * 
 * \defgroup rfm23bpw RFM23BPW
 * \ingroup drivers
 * \{
 */

#ifndef RFM23BPW_H_
#define RFM23BPW_H_

#include <stdint.h>
#include <config/config.h>
#include <libs/driverlib/driverlib.h>

#include "si4432/si4432.h"

#define RFM23BPW_SPI_CLK                BEACON_RADIO_SPI_CLK

/**
 * \defgroup pins RFM23BPW pins
 * \ingroup rfm23bpw
 * 
 * \brief MCU pins on RFM23BPW.
 * 
 * \{
 */
#define RFM23BPW_SPI_USCI               USCI_B          /**< USCI interface */
#define RFM23BPW_SPI_BASE_ADDRESS       USCI_B0_BASE    /**< Base address = USCI_B0_BASE */
#define RFM23BPW_SPI_PORT               GPIO_PORT_P2    /**< SPI port = P2 (all pins at the same port) */

#define RFM23BPW_NSEL_PORT              GPIO_PORT_P2    /**< NSEL port = P2 */
#define RFM23BPW_NSEL_PIN               GPIO_PIN0       /**< NSEL pin = P2.0 */

#define RFM23BPW_SDI_PORT               GPIO_PORT_P2    /**< SDI (MOSI) port = P2 */
#define RFM23BPW_SDI_PIN                GPIO_PIN1       /**< SDI (MOSI) pin = P2.1 */

#define RFM23BPW_SDO_PORT               GPIO_PORT_P2    /**< SDO (MISO) port = P2 */
#define RFM23BPW_SDO_PIN                GPIO_PIN2       /**< SDO (MISO) pin = P2.2 */

#define RFM23BPW_SCK_PORT               GPIO_PORT_P2    /**< SLK port = P2 */
#define RFM23BPW_SCK_PIN                GPIO_PIN3       /**< SLK pin = P2.3 */

#define RFM23BPW_SDN_PORT               GPIO_PORT_P2    /**< SDN port = P2 */
#define RFM23BPW_SDN_PIN                GPIO_PIN4       /**< SDN pin = P2.4 */

#define RFM23BPW_NIRQ_PORT              GPIO_PORT_P2    /**< NIRQ port = P2 */
#define RFM23BPW_NIRQ_PIN               GPIO_PIN4       /**< NIRQ pin = P2.4 */

#define RFM23BPW_GPIO0_PORT             GPIO_PORT_P1    /**< GPIO0 port = P1 */
#define RFM23BPW_GPIO0_PIN              GPIO_PIN6       /**< GPIO0 pin = P1.6 */

#define RFM23BPW_GPIO1_PORT             GPIO_PORT_P1    /**< GPIO1 port = P1 */
#define RFM23BPW_GPIO1_PIN              GPIO_PIN5       /**< GPIO1 pin = P1.5 */

#define RFM23BPW_GPIO2_PORT             GPIO_PORT_P1    /**< GPIO2 port = P1 */
#define RFM23BPW_GPIO2_PIN              GPIO_PIN5       /**< GPIO2 pin = P1.5 */

#define RFM23BPW_RXON_PORT              GPIO_PORT_P1    /**< RXON port = P1 */
#define RFM23BPW_RXON_PIN               GPIO_PIN5       /**< RXON pin = P1.5 */

#define RFM23BPW_TXON_PORT              GPIO_PORT_P1    /**< TXON port = P1 */
#define RFM23BPW_TXON_PIN               GPIO_PIN5       /**< TXON pin = P1.5 */
//! \} End of pins

/**
 * \fn rfm23bpw_Init
 * 
 * \brief 
 * 
 * \return 
 */
uint8_t rfm23bpw_Init();

#endif // RFM23BPW_H_

//! \} End of rfm23bpw group
