/*
 * beacon.h
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
 * \file beacon.h
 * 
 * \brief Beacon header.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/06/2017
 * 
 * \defgroup beacon Beacon
 * \{
 */

#ifndef BEACON_H_
#define BEACON_H_

#include <stdint.h>

#include <modules/modules.h>

#include "flags.h"
#include "packet_payload.h"

/**
 * \struct Beacon
 * 
 * \brief Beacon variables struct.
 */
typedef struct
{
    Flags           flags;                          /**< General flags. */
    uint8_t         energy_level;                   /**< Energy level of the satellite. */
    uint32_t        second_counter;                 /**< Seconds since boot. */
    uint32_t        last_radio_reset_time;          /**< Time stamp of the last radio reset. */
    uint32_t        last_system_reset_time;         /**< Time stamp of the last system reset. */
    uint32_t        last_ngham_pkt_transmission;    /**< Time stamp of the last NGHam packet transmission. */
    uint32_t        last_devices_verification;      /**< Time stamp of the last devices verification. */
    uint32_t        hibernation_mode_initial_time;  /**< Seconds since boot before the hibernations. */
    OBDH            obdh;                           /**< OBDH struct. */
    EPS             eps;                            /**< EPS struct. */
    PacketPayload   packet_payload;                 /**< Payload of the next packet to transmit. */
} Beacon;

/**
 * \var beacon
 * 
 * \brief Beacon variables struct.
 */
extern Beacon beacon;

/**
 * \fn beacon_init
 * 
 * \brief Beacon initialization routine.
 * 
 * \return None
 */
void beacon_init();

/**
 * \fn beacon_run
 * 
 * \brief Main operation routine.
 * 
 * \return None
 */
void beacon_run();

/**
 * \fn beacon_deinit
 * 
 * \brief Beacon deinitialization routine.
 * 
 * \return None
 */
void beacon_deinit();

/**
 * \fn beacon_shutdown
 * 
 * \brief Shutdowns the system.
 * 
 * \return None
 */
void beacon_shutdown();

/**
 * \fn beacon_reset
 * 
 * \brief Resets the system using a software BOR.
 * 
 * \return None
 */
void beacon_reset();

/**
 * \fn beacon_enter_hibernation
 * 
 * \brief Makes the beacon enter in hibernation.
 * 
 * \return None
 */
void beacon_enter_hibernation();

/**
 * \fn beacon_leave_hibernation
 * 
 * \brief Makes the beacon leave the hibernation.
 * 
 * \return None
 */
void beacon_leave_hibernation();

/**
 * \fn beacon_enter_low_power_mode
 * 
 * \brief Makes the MCU enter in low-power mode.
 * 
 * \return None
 */
void beacon_enter_low_power_mode();

/**
 * \fn beacon_get_tx_period
 * 
 * \brief Returns the TX period of the beacon (This value is dependent of the satellite energy level).
 * 
 * \return The TX period of the beacon.
 */
uint8_t beacon_get_tx_period();

/**
 * \fn beacon_generate_packet_payload
 * 
 * \brief Generates a packet payload from the OBDH or EPS data.
 * 
 * If the last OBDH data is not valid, the last valid EPS data is used to generate the packet payload.
 * 
 * \return None
 */
void beacon_generate_packet_payload();

/**
 * \fn beacon_generate_packets
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
void beacon_generate_packets(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len, uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len);

/**
 * \fn beacon_generate_ngham_packet
 * 
 * \brief Generates a payload and a NGHam packets to transmit.
 * 
 * \param ngham_pkt_str is a pointer to an array to store the NGHam packet.
 * \param ngham_pkt_str_len is a pointer to a byte to store the lenght of the NGHam packet.
 * 
 * \return None
 */
void beacon_generate_ngham_packet(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len);

/**
 * \fn beacon_generate_ax25_packet
 * 
 * \brief Generates a payload and an AX.25 packet to transmit.
 * 
 * \param ax25_pkt_str is a pointer to an array to store the AX.25 packet.
 * \param ax25_pkt_str_lens is a pointer to a byte to store the lenght of the AX.25 packet.
 * 
 * \return None
 */
void beacon_generate_ax25_packet(uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len);

/**
 * \fn beacon_transmit_ngham_packet
 * 
 * \brief Transmit a beacon packet using the NGHam protocol.
 * 
 * \return None
 */
void beacon_transmit_ngham_packet();

/**
 * \fn beacon_transmit_ax25_packet
 * 
 * \brief Transmit a beacon packet using the AX.25 protocol.
 * 
 * \return None
 */
void beacon_transmit_ax25_packet();

/**
 * \fn beacon_set_energy_level
 * 
 * \brief Sets the beacon energy level (From the data received from the OBDH or EPS modules).
 * 
 * If both OBDH and EPS are not sending data anymore, the smaller energy level is chosen.
 * 
 * \return None
 */
void beacon_set_energy_level();

/**
 * \fn beacon_check_devices_status
 * 
 * \brief Checks if the system devices or modules (OBDH, EPS, antenna, radio, etc.) are working or not.
 * 
 * \return None
 */
void beacon_check_devices_status();

/**
 * \fn beacon_process_received_packet_data
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
void beacon_process_received_packet_data(uint8_t *data, uint8_t len);

/**
 * \fn beacon_antenna_deployment
 * 
 * \brief Antenna deployment procedure.
 * 
 * \return None
 */
void beacon_antenna_deployment();

/**
 * \fn beacon_receive_packet
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
void beacon_receive_packet(uint8_t *pkt, uint8_t len);

#endif // BEACON_H_

//! \} End of beacon group
