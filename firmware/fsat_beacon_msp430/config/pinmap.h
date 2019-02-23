/*
 * pinmap.h
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
 * \brief Hardware pin mapping on the MCU.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 08/06/2017
 * 
 * \defgroup pinmap Pinmap
 * \ingroup config
 * \{
 */

#ifndef PINMAP_H_
#define PINMAP_H_

#include <drivers/driverlib/driverlib.h>

//##################################################
//-- COMMON ----------------------------------------
//##################################################

#define USCI_A                      0
#define USCI_B                      1

//##################################################
//-- RADIO -----------------------------------------
//##################################################

#define RADIO_SPI_PORT              GPIO_PORT_P2
#define RADIO_SPI_USCI              USCI_B
#define RADIO_SPI_BASE_ADDRESS      USCI_B0_BASE

#define RADIO_SPI_NSEL_PORT         GPIO_PORT_P2
#define RADIO_SPI_NSEL_PIN          GPIO_PIN0

#define RADIO_SPI_MOSI_PORT         GPIO_PORT_P2
#define RADIO_SPI_MOSI_PIN          GPIO_PIN1

#define RADIO_SPI_MISO_PORT         GPIO_PORT_P2
#define RADIO_SPI_MISO_PIN          GPIO_PIN2

#define RADIO_SPI_SCLK_PORT         GPIO_PORT_P2
#define RADIO_SPI_SCLK_PIN          GPIO_PIN3

#define RADIO_GPIO_SDN_PORT         GPIO_PORT_P3
#define RADIO_GPIO_SDN_PIN          GPIO_PIN0

#define RADIO_GPIO_nIRQ_PORT        GPIO_PORT_P3
#define RADIO_GPIO_nIRQ_PIN         GPIO_PIN1
#define RADIO_GPIO_nIRQ_ISR_VECTOR  PORT3_VECTOR

#define RADIO_GPIO_0_PORT           GPIO_PORT_P3
#define RADIO_GPIO_0_PIN            GPIO_PIN2

#define RADIO_GPIO_1_PORT           GPIO_PORT_P3
#define RADIO_GPIO_1_PIN            GPIO_PIN3

#define RADIO_POWER_ENABLE_PORT     GPIO_PORT_P1
#define RADIO_POWER_ENABLE_PIN      GPIO_PIN1

#define RADIO_SIM_UART_USCI         USCI_A
#define RADIO_SIM_UART_BASE_ADDRESS USCI_A0_BASE
#define RADIO_SIM_UART_TX_PORT      GPIO_PORT_P2
#define RADIO_SIM_UART_TX_PIN       GPIO_PIN4

//##################################################
//-- PA --------------------------------------------
//##################################################

#define PA_GPIO_EN_PORT             GPIO_PORT_P1
#define PA_GPIO_EN_PIN              GPIO_PIN1

#define PA_DAC_GAIN_PORT            GPIO_PORT_P6
#define PA_DAC_GAIN_PIN             GPIO_PIN6

//##################################################
//-- RF SWITCH -------------------------------------
//##################################################

#define RF_SWITCH_GPIO_CTRL_PORT    GPIO_PORT_P5
#define RF_SWITCH_GPIO_CTRL_PIN     GPIO_PIN5

//##################################################
//-- ANTENNA ---------------------------------------
//##################################################

#define ANTENNA_I2C_PORT            GPIO_PORT_P9
#define ANTENNA_I2C_USCI            USCI_B
#define ANTENNA_I2C_BASE_ADDRESS    USCI_B2_BASE

#define ANTENNA_I2C_SDA_PORT        GPIO_PORT_P9
#define ANTENNA_I2C_SDA_PIN         GPIO_PIN5

#define ANTENNA_I2C_SCL_PORT        GPIO_PORT_P9
#define ANTENNA_I2C_SCL_PIN         GPIO_PIN6

#define ANTENNA_GPIO_DEBUG_PORT     GPIO_PORT_P9
#define ANTENNA_GPIO_DEBUG_PIN      GPIO_PIN7

//##################################################
//-- EPS -------------------------------------------
//##################################################

#define EPS_UART_PORT               GPIO_PORT_P2
#define EPS_UART_USCI               USCI_A
#define EPS_UART_BASE_ADDRESS       USCI_A0_BASE
#define EPS_UART_ISR_VECTOR         USCI_A0_VECTOR

#define EPS_UART_RX_PORT            GPIO_PORT_P2
#define EPS_UART_RX_PIN             GPIO_PIN5

//##################################################
//-- OBDH ------------------------------------------
//##################################################

#define OBDH_SPI_PORT               GPIO_PORT_P9
#define OBDH_SPI_USCI               USCI_A
#define OBDH_SPI_BASE_ADDRESS       USCI_A2_BASE

#define OBDH_SPI_NSEL_PORT          GPIO_PORT_P9
#define OBDH_SPI_NSEL_PIN           GPIO_PIN4

#define OBDH_SPI_MOSI_PORT          GPIO_PORT_P9
#define OBDH_SPI_MOSI_PIN           GPIO_PIN2

#define OBDH_SPI_MISO_PORT          GPIO_PORT_P9
#define OBDH_SPI_MISO_PIN           GPIO_PIN3

#define OBDH_SPI_SCLK_PORT          GPIO_PORT_P9
#define OBDH_SPI_SCLK_PIN           GPIO_PIN1

//##################################################
//-- STATUS LED ------------------------------------
//##################################################

#define STATUS_LED_GPIO_PORT        GPIO_PORT_P5
#define STATUS_LED_GPIO_PIN         GPIO_PIN4

//##################################################
//-- DEBUG -----------------------------------------
//##################################################

#define SYS_DEBUG_UART_BASE_ADDRESS     USCI_A1_BASE
#define SYS_DEBUG_UART_PORT             GPIO_PORT_P8

#define SYS_DEBUG_UART_TX_PORT          GPIO_PORT_P8
#define SYS_DEBUG_UART_TX_PIN           GPIO_PIN2

#define SYS_DEBUG_UART_RX_PORT          GPIO_PORT_P8
#define SYS_DEBUG_UART_RX_PIN           GPIO_PIN3

#endif // PINMAP_H_

//! \} End of pinmap group
