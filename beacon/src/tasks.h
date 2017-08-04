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
 * \fn task_transmit_packet
 * 
 * \brief Transmit a beacon packet
 * 
 * \return None
 */
void task_transmit_packet();

/**
 * \fn task_generate_packets()
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
 * \fn task_check_elapsed_time
 * 
 * \brief Returns the elapsed time between a interval in a specified time unit.
 * 
 * \param initial_time is the initial time.
 * \param final_time is the final time.
 * \param time_unit is the time unit to calc. the elased time.
 * 
 * \return The elapsed time between the initial and final time in specified time unit.
 */
uint16_t task_check_elapsed_time(uint16_t initial_time, uint16_t final_time, uint8_t time_unit);

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
 * \return The TX period of the beacon.
 */
uint8_t task_get_tx_period();

/**
 * \fn task_generate_packet_payload
 * 
 * \brief Generates a packet payload from the OBDH or EPS data.
 * 
 * If the last OBDH data is not valid, the last valid EPS data is used to generate the packet payload.
 * 
 * \param b is a pointer to a Beacon struct.
 * 
 * \return None
 */
void task_generate_packet_payload(Beacon *b);

#endif // TASKS_H_

//! \} End of tasks group