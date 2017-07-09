/*
 * gpio_obdh.h
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
 * \file obdh_com.h
 * 
 * \brief OBDH communication routines.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \defgroup obdh_com OBDH Communication
 * \ingroup modules
 * \{
 */

#ifndef OBDH_COM_H_
#define OBDH_COM_H_

#include <stdint.h>

/**
 * \struct OBDHCom
 * 
 * \brief OBDH communication variables struct.
 */
struct OBDHCom
{
    uint8_t received_byte;                      /**< Byte buffer. */
    uint8_t byte_counter;                       /**< Received packet byte counter. */
    uint8_t buffer[OBDH_COM_DATA_PKT_LEN + 1];  /**< Packet buffer. */
};

/**
 * \struct OBDHData
 * 
 * \brief OBDH data packet.
 */
struct OBDHData
{
    uint8_t v_bat1[3];              /**< Battery 1 voltage. */
    uint8_t v_bat2[3];              /**< Battery 2 voltage. */
    uint8_t i_solar_panels[13];     /**< Solar panels currents. */
    uint8_t v_solar_panels[7];      /**< Solar panels voltages. */
    uint8_t t_bats[7];              /**< Batteries temperatures. */
    uint8_t imu[13];                /**< Accelerometer and gyroscope data. */
    uint8_t q_bats[3];              /**< Total charge of the batteries. */
    uint8_t system_time[5];         /**< Time since last boot. */
    uint8_t sat_status[3];          /**< Overall status of the satellite. */
    uint8_t reset_counter[3];       /**< Number of OBDH resets since launch. */
};

/**
 * \var obdh_com
 * 
 * \brief OBDH communication data.
 */
extern OBDHCom obdh_com;

/**
 * \var obdh_data
 * 
 * \brief Last data packet received from the OBDH module.
 */
extern OBDHData obdh_data;

/**
 * \fn obdh_com_init
 * 
 * \brief Initialization of the OBDH communication.
 * 
 * \return Initialization status. It can be:
 *              -\b STATUS_SUCCESS
 *              -\b STATUS_FAIL
 *              .
 */
uint8_t obdh_com_init();

/**
 * \fn obdh_com_spi_init
 * 
 * \brief OBDH communication SPI initialization.
 * 
 * \return Initialization status. It can be:
 *              -\b STATUS_SUCCESS
 *              -\b STATUS_FAIL
 *              .
 */
static uint8_t obdh_com_spi_init();

/**
 * \fn obdh_com_receive_data
 * 
 * \brief Receives data from the OBDH interruption.
 * 
 * \return None
 */
static void obdh_com_receive_data();

/**
 * \fn obdh_com_receive_cmd
 * 
 * \brief Receives and verifies a command from the OBDH module.
 * 
 * \param cmd is the command to be received.
 * 
 * \return None
 */
static void obdh_com_receive_cmd(uint8_t cmd);

/**
 * \fn obdh_com_receive_pkt
 * 
 * \brief Receives a data packet from the OBDH module.
 * 
 * \param byte is a byte of the incoming packet.
 * 
 * \return None
 */
static void obdh_com_receive_pkt(uint8_t byte);

/**
 * \fn obdh_com_send_data
 * 
 * \brief Sends a byte of data to the OBDH module.
 * 
 * This function is used to send acknowledge and the RF channel state.
 * 
 * \param data is the byte to send to the OBDH module.
 * 
 * \return None
 */
void obdh_com_send_data(uint8_t data);

/**
 * \fn obdh_com_save_data_from_buffer
 * 
 * \brief Copy the bytes of the buffer to the obdh_data struct.
 * 
 * \return None
 */
static void obdh_com_save_data_from_buffer();

/**
 * \fn obdh_com_clear_buffer
 * 
 * \brief Clears the data packet buffer.
 * 
 * \return None
 */
static void obdh_com_clear_buffer();

#endif // OBDH_COM_H_

//! \} End of obdh_com group
