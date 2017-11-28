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
#include <stdbool.h>

#include <system/buffer/buffer.h>

#include "fsat_module.h"

/**
 * \struct Beacon
 * 
 * \brief Beacon variables struct.
 */
typedef struct
{
    bool        hibernation;                    /**< If true, the beacon is in hibernation mode, otherwise, not. */
    bool        can_transmit;                   /**< If true, the beacon can transmit packets, otherwise, not. */
    bool        transmitting;                   /**< If true, the beacon is transmitting packets, otherwise, not. */
    uint8_t     energy_level;                   /**< Energy level of the satellite. */
    uint32_t    last_radio_reset_time;          /**< Time stamp of the last radio reset. */
    uint32_t    last_system_reset_time;         /**< Time stamp of the last system reset. */
    uint32_t    last_ngham_pkt_transmission;    /**< Time stamp of the last NGHam packet transmission. */
    uint32_t    last_devices_verification;      /**< Time stamp of the last devices verification. */
    uint32_t    last_energy_level_set;          /**< Time stamp of the last energy level verification. */
    uint32_t    hibernation_mode_initial_time;  /**< Seconds since boot before the hibernations. */
    uint32_t    time_obdh_started_tx;           /**< Time stamp of the allowed window to OBDH transmit data via radio. */
    Buffer      radio_rx;                       /**< Radio RX data buffer. */
    Buffer      pkt_payload;                    /**< The current payload to transmit in a packet (With the OBDH or EPS data, or only with the satellite ID). */
    FSatModule  obdh;                           /**< OBDH module. */
    FSatModule  eps;                            /**< EPS module. */
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
 * \fn beacon_deinit
 * 
 * \brief Beacon deinitialization routine.
 * 
 * \return None
 */
void beacon_deinit();

/**
 * \fn beacon_run
 * 
 * \brief Main operation routine.
 * 
 * \return None
 */
void beacon_run();

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
 * \fn beacon_get_tx_period
 * 
 * \brief Returns the TX period of the beacon (This value is dependent of the satellite energy level).
 * 
 * \return The TX period of the beacon.
 */
uint8_t beacon_get_tx_period();

/**
 * \fn beacon_gen_pkt_payload
 * 
 * \brief Generates a packet payload from the OBDH or EPS data.
 * 
 * If the last OBDH data is not valid, the last valid EPS data is used to generate the packet payload.
 * 
 * \return None
 */
void beacon_gen_pkt_payload();

/**
 * \fn beacon_gen_ngham_pkt
 * 
 * \brief Generates a payload and a NGHam packets to transmit.
 * 
 * \param ngham_pkt_str is a pointer to an array to store the NGHam packet.
 * \param ngham_pkt_str_len is a pointer to a byte to store the lenght of the NGHam packet.
 * 
 * \return None
 */
void beacon_gen_ngham_pkt(uint8_t *ngham_pkt_str, uint16_t *ngham_pkt_str_len);

/**
 * \fn beacon_gen_ax25_pkt
 * 
 * \brief Generates a payload and an AX.25 packet to transmit.
 * 
 * \param ax25_pkt_str is a pointer to an array to store the AX.25 packet.
 * \param ax25_pkt_str_lens is a pointer to a byte to store the lenght of the AX.25 packet.
 * 
 * \return None
 */
void beacon_gen_ax25_pkt(uint8_t *ax25_pkt_str, uint16_t *ax25_pkt_str_len);

/**
 * \fn beacon_send_ngham_pkt
 * 
 * \brief Transmit a beacon packet using the NGHam protocol.
 * 
 * \return None
 */
void beacon_send_ngham_pkt();

/**
 * \fn beacon_send_ax25_pkt
 * 
 * \brief Transmit a beacon packet using the AX.25 protocol.
 * 
 * \return None
 */
void beacon_send_ax25_pkt();

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
 * \fn beacon_process_radio_pkt
 * 
 * \brief Process an incoming packet payload from the radio.
 * 
 * Verifies if the received data is a valid command, and executes it.
 * 
 * \return None
 */
void beacon_process_radio_pkt();

/**
 * \fn beacon_process_obdh_pkt
 * 
 * \brief Processes a packet from the OBDH module.
 * 
 * \return None
 */
void beacon_process_obdh_pkt();

/**
 * \fn beacon_process_eps_pkt
 * 
 * \brief Processes a packet from the EPS module.
 * 
 * \return None
 */
void beacon_process_eps_pkt();

/**
 * \fn beacon_antenna_deployment
 * 
 * \brief Antenna deployment procedure.
 * 
 * \return None
 */
void beacon_antenna_deployment();

#endif // BEACON_H_

//! \} End of beacon group
