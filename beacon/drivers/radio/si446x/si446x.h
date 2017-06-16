/*
 * si446x.h
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
 * \file si446x.h
 * 
 * \brief Silicon Labs SI446x driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 01/06/2017
 * 
 * \defgroup si446x SI446X
 * \ingroup drivers
 * \{
 */

#ifndef SI446X_H_
#define SI446X_H_

#include <stdint.h>

/**
 * \defgroup states SI446x states
 * \ingroup si446x
 * 
 * \brief SI446x operating modes (states).
 * 
 * \{
 */
#define SI446X_STATE_POR                0x00
#define SI446X_STATE_SHUTDOWN           0x01
#define SI446X_STATE_STANDBY            0x02
#define SI446X_STATE_SLEEP              0x03
#define SI446X_STATE_SPI_ACTIVE         0x04
#define SI446X_STATE_READY              0x05
#define SI446X_STATE_TX                 0x06
#define SI446X_STATE_RX                 0x07
//! \} End of states

/**
 * \defgroup cmds Commands
 * \ingroup si446x
 * 
 * \brief API commands.
 * 
 * SPI transactions of sending commands and getting information from the chip.
 * 
 * \{
 */
// BOOT_COMMANDS
#define SI446X_CMD_POWER_UP             0x02    /**< Command to power-up the device and select the operational mode and functionality. */
// COMMON_COMMANDS
#define SI446X_CMD_NOP                  0x00    /**< No operation command. */
#define SI446X_CMD_PART_INFO            0x01    /**< Reports basic information about the device. */
#define SI446X_CMD_FUNC_INFO            0x10    /**< Returns the Function revision information of the device. */
#define SI446X_CMD_SET_PROPERTY         0x11    /**< Sets the value of one or more properties. */
#define SI446X_CMD_GET_PROPERTY         0x12    /**< Retrieves the value of one or more properties. */
#define SI446X_CMD_GPIO_PIN_CFG         0x13    /**< Configures the GPIO pins. */
#define SI446X_CMD_FIFO_INFO            0x15    /**< Access the current byte counts in the TX and RX FIFOs and provide for resetting the FIFOs. */
#define SI446X_CMD_GET_INT_STATUS       0x20    /**< Returns the interrupt status of ALL the possible interrupt events (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */
#define SI446X_CMD_REQUEST_DEVICE_STATE 0x33    /**< Request current device state and channel. */
#define SI446X_CMD_CHANGE_STATE         0x34    /**< Manually switch the chip to a desired operating state. */
#define SI446X_CMD_OFFLINE_RECAL        0x38    /**< Recalibrates due to temperature change. */
#define SI446X_CMD_READ_CMD_BUFF        0x44    /**< Used to read CTS and the command response. */
#define SI446X_CMD_FRR_A_READ           0x50    /**< Reads the fast response registers (FRR) starting with FRR_A. */
#define SI446X_CMD_FRR_B_READ           0x51    /**< Reads the fast response registers (FRR) starting with FRR_B. */
#define SI446X_CMD_FRR_C_READ           0x53    /**< Reads the fast response registers (FRR) starting with FRR_C. */
#define SI446X_CMD_FRR_D_READ           0x57    /**< Reads the fast response registers (FRR) starting with FRR_D. */
// IR_CAL_COMMANDS
#define SI446X_CMD_IRCAL                0x17    /**< Image rejection calibration. */
#define SI446X_CMD_IRCAL_MANUAL         0x19    /**< Image rejection calibration. */
// TX_COMMANDS
#define SI446X_CMD_START_TX             0x31    /**< Switches to TX state and starts transmission of a packet. */
#define SI446X_CMD_TX_HOP               0x37    /**< Hop to a new frequency while in TX. */
#define SI446X_CMD_WRITE_TX_FIFO        0x66    /**< Writes data byte(s) to the TX FIFO. */
// RX_COMMANDS
#define SI446X_CMD_PACKET_INFO          0x16    /**< Returns information about the length of the variable field in the last packet received and (optionally) overrides field length. */
#define SI446X_CMD_GET_MODEM_STATUS     0x22    /**< Returns the interrupt status of the Modem Interrupt Group (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */
#define SI446X_CMD_START_RX             0x32    /**< Switches to RX state and starts reception of a packet. */
#define SI446X_CMD_RX_HOP               0x36    /**< Manually hop to a new frequency while in RX mode. */
#define SI446X_CMD_READ_RX_FIFO         0x77    /**< Reads data byte(s) from the RX FIFO. */
// ADVANCED_COMMANDS
#define SI446X_CMD_GET_ADC_READING      0x14    /**< Performs conversions using the Auxiliary ADC and returns the results of those conversions. */
#define SI446X_CMD_GET_PH_STATUS        0x21    /**< Returns the interrupt status of the Packet Handler Interrupt Group (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */
#define SI446X_CMD_GET_CHIP_STATUS      0x23    /**< Returns the interrupt status of the Chip Interrupt Group (both STATUS and PENDING). Optionally, it may be used to clear latched (PENDING) interrupt events. */
//! \} End of cmds group

/**
 * \fn si446x_init
 * 
 * \brief Initializes the radio.
 * 
 * \return 
 */
uint8_t si446x_init();

/**
 * \fn si446x_spi_init
 * 
 * \brief Initializes the SPI interface.
 * 
 * \return 
 */
uint8_t si446x_spi_init();

/**
 * \fn si446x_config
 * 
 * \brief 
 * 
 * \return 
 */
uint8_t si446x_config();

/**
 * \fn si446x_set_state
 * 
 * \brief Sets a given state in the radio.
 * 
 * \param state is the state to put the radio.
 * 
 * \return None
 */
void si446x_set_state(uint8_t state);

/**
 * \fn si446x_cmd
 * 
 * \brief 
 * 
 * \param cmd
 * 
 * \return None
 */
void si446x_cmd(uint8_t cmd);

/**
 * \fn si446x_write_tx_fifo
 * 
 * \brief 
 * 
 * \param 
 * 
 * \return None
 */
void si446x_write_tx_fifo();

/**
 * \fn si446x_read_rx_fifo
 * 
 * \brief 
 * 
 * \param 
 * 
 * \return None
 */
void si446x_read_rx_fifo();

#endif // SI446X_H_

//! \} End of si446x group
