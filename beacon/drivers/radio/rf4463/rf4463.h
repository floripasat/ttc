/*
 * rf4463.h
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
 * \file rf4463.h
 * 
 * \brief NiceRF RF4463 driver.
 * 
 * This library suits for RF4463PRO and RF4463F30 in FIFO mode.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 01/06/2017
 * 
 * \defgroup rf4463 RF4463
 * \ingroup drivers
 * \{
 */

#ifndef RF4463_H_
#define RF4463_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * \fn rf4463_init
 * 
 * \brief RF4463 initialization.
 * 
 * Initializes the RF4463 module with the configuration parameters from
 * "rf4463_reg_config.h".
 * 
 * \return Initialization status. It can be:
 *              -\b STATUS_SUCCESS if the initialization was successful
 *              -\b STATUS_FAIL if the initialization was not successful
 *              .
 */
uint8_t rf4463_init();

/**
 * \fn rf4463_gpio_init
 * 
 * \brief RF4463F30 GPIO initialization.
 * 
 * \return None
 */
static void rf4463_gpio_init();

/**
 * \fn rf4463_spi_init
 * 
 * \brief RF4463 SPI interface initialization.
 * 
 * \return Initialization status. It can be:
 *              -\b STATUS_SUCCESS
 *              -\b STATUS_FAIL
 *              .
 */
static uint8_t rf4463_spi_init();

/**
 * \fn rf4463_reset
 * 
 * \brief Resets the RF4463 module.
 * 
 * \return None
 */
void rf4463_reset();

/**
 * \fn rf4463_tx_packet
 * 
 * \brief Transmit a packet through RF.
 * 
 * \param send_buf is the data with the packet to send.
 * \param send_len is the length of the data to send.
 * 
 * \return It can return:
 *              -\b true if the packet was sent.
 *              -\b false if the packet was not sent.
 *              .
 */
bool rf4463_tx_packet(uint8_t *send_buf, uint8_t send_len);

/**
 * \fn rf4463_rx_packet
 * 
 * \brief 
 * 
 * \param rx_buf
 * 
 * \return 
 */
uint8_t rf4463_rx_packet(uint8_t *rx_buf);

/**
 * \fn rf4463_rx_init
 * 
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_rx_init();

/**
 * \fn rf4463_spi_write_byte
 * 
 * \brief Transfers a byte through the SPI interface.
 * 
 * \param byte is the byte to be transferes.
 * 
 * \return None
 */
void rf4463_spi_write_byte(uint8_t byte);

/**
 * \fn rf4463_spi_write
 * 
 * \brief Transfers an array through the SPI interface.
 * 
 * \param data is an array to be  transfered.
 * \param size is the size of the data to be transfered.
 * 
 * \return None
 */
void rf4463_spi_write(uint8_t *data, uint16_t size);

/**
 * \fn rf4463_spi_read_byte
 * 
 * \brief Reads a byte from the SPI interface.
 * 
 * \return The byte read from the SPI interface.
 */
uint8_t rf4463_spi_read_byte();

/**
 * \fn rf4463_spi_read
 * 
 * \brief Reads data from the SPI interface
 * 
 * \param data is a pointer to where the incoming data will be stored.
 * \param size is how many bytes will be read from the SPI interface.
 * 
 * \return None
 */
void rf4463_spi_read(uint8_t *data, uint16_t size);

/**
 * \fn rf4463_check_device
 * 
 * \brief Checks if the device is working.
 * 
 * \return It can return:
 *              -\b true if the device is working.
 *              -\b false if the device is not working.
 *              .
 */
bool rf4463_check_device();

/**
 * \fn rf4463_check_cts
 * 
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_check_cts();

/**
 * \fn rf4463_set_tx_power
 * 
 * \brief 
 * 
 * \param pwr
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_tx_power(uint8_t pwr);

/**
 * \fn rf4463_set_properties
 * 
 * \brief 
 * 
 * \param start_property
 * \param length
 * \param para_buf
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf);

/**
 * \fn rf4463_get_properties
 * 
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_get_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf);

/**
 * \fn rf4463_set_config
 * 
 * \brief 
 * 
 * \param 
 * \param 
 * 
 * \return None
 */
void rf4463_set_config(const uint8_t *parameters, uint16_t para_len);

/**
 * \fn rf4463_set_preamble_len
 * 
 * \brief 
 * 
 * \param len
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_preamble_len(uint8_t len);

/**
 * \fn rf4463_set_sync_word
 * 
 * \brief 
 * 
 * \param sync_word
 * \param len
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_sync_word(uint8_t *sync_word, uint8_t len);

/**
 * \fn rf4463_set_gpio_mode
 * 
 * \brief 
 * 
 * \param gpio0_mode
 * \param gpio1_mode
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_gpio_mode(uint8_t gpio0_mode, uint8_t gpio1_mode);

/**
 * \fn rf4463_set_cmd
 * 
 * \brief 
 * 
 * \param len
 * \param cmd
 * \param para_buf
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_cmd(uint8_t len, uint8_t cmd, uint8_t para_buf);

/**
 * \fn rf4463_get_cmd
 * 
 * \brief Reads a command.
 * 
 * \param length is the length of the parameter to read.
 * \param cmd is the command to read.
 * \param para_buf is buffer to store the parameters.
 * 
 * \return It can return:
 *              -\b true if no error occurs.
 *              -\b false if an error occurs.
 *              .
 */
bool rf4463_get_cmd(uint8_t length, uint8_t cmd, uint8_t *para_buf);

/**
 * \fn rf4463_set_tx_interrupt
 * 
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_tx_interrupt();

/**
 * \fn rf4463_set_rx_interrupt
 * 
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_set_rx_interrupt();

/**
 * \fn rf4463_clear_interrupts
 * 
 * \brief 
 * 
 * \return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_clear_interrupts();

/**
 * \fn rf4463_write_tx_fifo
 * 
 * \brief 
 * 
 * \param data
 * \param len
 * 
 * \return None
 */
void rf4463_write_tx_fifo(uint8_t *data, uint8_t len);

/**
 * \fn rf4463_read_rx_fifo
 * 
 * \brief 
 * 
 * \param data
 * 
 * \return 
 */
uint8_t rf4463_read_rx_fifo(uint8_t *data);

/**
 * \fn rf4463_fifo_reset
 * 
 * \brief 
 * 
 * \return None
 */
void rf4463_fifo_reset();

/**
 * \fn rf4463_enter_tx_mode
 * 
 * \brief 
 * 
 * \return None
 */
void rf4463_enter_tx_mode();

/**
 * \fn rf4463_enter_rx_mode
 * 
 * \brief 
 * 
 * \return None
 */
void rf4463_enter_rx_mode();

/**
 * \fn rf4463_enter_standby_mode
 * 
 * \brief 
 * 
 * return It can return:
 *              -\b true 
 *              -\b false 
 *              .
 */
bool rf4463_enter_standby_mode();

/**
 * \fn rf4463_wait_nIRQ
 * 
 * \brief Inquire interrupt.
 * 
 * \return It can return:
 *              -\b true if an interrupt occurs.
 *              -\b false if no interrupt occurs.
 *              .
 */
bool rf4463_wait_nIRQ();

/**
 * \fn rf4463_turn_on_pa
 * 
 * \brief Turns on the PA.
 * 
 * \return None
 */
void rf4463_turn_on_pa();

/**
 * \fn rf4463_turn_off_pa
 * 
 * \brief Turns off the PA.
 * 
 * \return None
 */
void rf4463_turn_off_pa();

#endif // RF4463_H_

//! \} End of rf4463 group
