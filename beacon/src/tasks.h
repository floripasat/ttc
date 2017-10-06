/*
 * tasks.h
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
 * \file tasks.h
 * 
 * \brief Beacon tasks.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \defgroup tasks Beacon Tasks
 * \ingroup src
 * \{
 */

#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>

#include "beacon.h"

/**
 * \fn task_transmit_ngham_packet
 * 
 * \brief Transmit a beacon packet using the NGHam protocol.
 * 
 * \param beacon_ptr is a pointer to a Beacon object.
 * 
 * \return None
 */
void task_transmit_ngham_packet(Beacon *beacon_ptr);

/**
 * \fn task_transmit_ax25_packet
 * 
 * \brief Transmit a beacon packet using the AX.25 protocol.
 * 
 * \param beacon_ptr is a pointer to a Beacon object.
 * 
 * \return None
 */
void task_transmit_ax25_packet(Beacon *beacon_ptr);

/**
 * \fn task_receive_packet
 * 
 * \brief Receives a incoming packet.
 * 
 * This function decodes a packet (with the NGHam protocol) and executes its command.
 * 
 * \param pkt is an array with the received raw packet.
 * \param len is the lenght of the received packet.
 * 
 * \return None
 */
void task_receive_packet(uint8_t *pkt, uint8_t len);

/**
 * \fn task_process_received_packet_data
 * 
 * \brief Process an incoming packet payload.
 * 
 * Verifies if the received data is a valid command, and executes it.
 * 
 * \param data is the packet payload to process.
 * \param len is the lenght of the packet payload to process.
 * 
 * \return None
 */
void task_process_received_packet_data(uint8_t *data, uint8_t len);

/**
 * \fn task_generate_packets
 * 
 * \brief Generates the payload and the packets to transmit.
 * 
 * \param ngham_pkt_str is a pointer to an array to store the NGHam packet.
 * \param ngham_pkt_str_len is a pointer to a byte to store the lenght of the NGHam packet.
 * \param ax25_pkt_str is a pointer to an array to store the AX.25 packet.
 * \param ax25_pkt_str_lens is a pointer to a byte to store the lenght of the AX.25 packet.
 * 
 * \return None
 */
void task_generate_packets(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len, uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len);

/**
 * \fn task_generate_ngham_packet
 * 
 * \brief Generates a payload and a NGHam packets to transmit.
 * 
 * \param ngham_pkt_str is a pointer to an array to store the NGHam packet.
 * \param ngham_pkt_str_len is a pointer to a byte to store the lenght of the NGHam packet.
 * 
 * \return None
 */
void task_generate_ngham_packet(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len);

/**
 * \fn task_generate_ax25_packet
 * 
 * \brief Generates a payload and an AX.25 packet to transmit.
 * 
 * \param ax25_pkt_str is a pointer to an array to store the AX.25 packet.
 * \param ax25_pkt_str_lens is a pointer to a byte to store the lenght of the AX.25 packet.
 * 
 * \return None
 */
void task_generate_ax25_packet(uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len);

/**
 * \fn task_enter_low_power_mode
 * 
 * \brief Makes the MCU enter in low-power mode.
 * 
 * \return None
 */
void task_enter_low_power_mode();

/**
 * \fn tasl_leave_low_power_mode
 * 
 * \brief Makes the MCU leave low-power mode.
 * 
 * \return None
 */
void task_leave_low_power_mode();

/**
 * \fn task_enter_hibernation
 * 
 * \brief Makes the beacon enter in hibernation.
 * 
 * \return None
 */
void task_enter_hibernation();

/**
 * \fn task_leave_hibernation
 * 
 * \brief Makes the beacon leave the hibernation.
 * 
 * \return None
 */
void task_leave_hibernation();

/**
 * \fn task_save_time
 * 
 * \brief Saves the actual time in the flash memory.
 * 
 * \return None
 */
void task_save_time();

/**
 * \fn task_load_time
 * 
 * \brief Loads the saved time from the flash memory.
 * 
 * \return None
 */
void task_load_time();

/**
 * \fn task_reset_system
 * 
 * \brief Resets (Reinitializes) the system.
 * 
 * \return None
 */
void task_reset_system();

/**
 * \fn task_antenna_deployment
 * 
 * \brief Antenna deployment procedure.
 * 
 * \return None
 */
void task_antenna_deployment();

/**
 * \fn task_set_energy_level
 * 
 * \brief Sets the beacon energy level (From the data received from the OBDH or EPS modules).
 * 
 * \param beacon_prt is a pointer to a beacon object.
 * 
 * \return None
 */
void task_set_energy_level(Beacon *beacon_ptr);

/**
 * \fn task_get_tx_period
 * 
 * \brief Returns the TX period of the beacon (This value is dependent of the satellite energy level).
 * 
 * \param beacon_ptr is a pointer to a Beacon struct.
 * 
 * \return The TX period of the beacon.
 */
uint8_t task_get_tx_period(Beacon *beacon_ptr);

/**
 * \fn task_generate_packet_payload
 * 
 * \brief Generates a packet payload from the OBDH or EPS data.
 * 
 * If the last OBDH data is not valid, the last valid EPS data is used to generate the packet payload.
 * 
 * \param beacon_ptr is a pointer to a Beacon struct.
 * 
 * \return None
 */
void task_generate_packet_payload(Beacon *beacon_ptr);

/**
 * \fn task_enable_rx
 * 
 * \brief If available, enables the reception in the radio module.
 * 
 * \return None
 */
void task_enable_rx();

/**
 * \fn task_disable_rx
 * 
 * \brief Disables the RX of the radio (If available, enables the standby mode).
 * 
 * \return None
 */
void task_disable_rx();

/**
 * \fn task_reset_radio
 * 
 * \brief Resets the radio periodically.
 * 
 * \param beacon_ptr is the beacon struct.
 * 
 * \return None
 */
void task_reset_radio(Beacon *beacon_ptr);

#endif // TASKS_H_

//! \} End of tasks group
