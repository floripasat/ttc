/*
 * isis_antenna.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief ISIS antenna driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 08/07/2017
 * 
 * \defgroup isis_antenna ISIS antenna
 * \ingroup drivers
 * \{
 */

#ifndef ISIS_ANTENNA_H_
#define ISIS_ANTENNA_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * \brief ISIS antenna status.
 */
typedef struct
{
    uint8_t antenna_1_status  : 1;      /**< . */
    uint8_t antenna_1_timeout : 1;      /**< . */
    uint8_t antenna_1_burning : 1;      /**< . */
    uint8_t antenna_2_status  : 1;      /**< . */
    uint8_t antenna_2_timeout : 1;      /**< . */
    uint8_t antenna_2_burning : 1;      /**< . */
    uint8_t ignoring_switches : 1;      /**< . */
    uint8_t antenna_3_status  : 1;      /**< . */
    uint8_t antenna_3_timeout : 1;      /**< . */
    uint8_t antenna_3_burning : 1;      /**< . */
    uint8_t independent_burn  : 1;      /**< . */
    uint8_t antenna_4_status  : 1;      /**< . */
    uint8_t antenna_4_timeout : 1;      /**< . */
    uint8_t antenna_4_burning : 1;      /**< . */
    uint8_t armed             : 1;      /**< . */
} ISIS_Antenna_Status;

#define ISIS_ANTENNA_STATUS_MASK            0x8888  /**< . */

// Antenna status
#define ISIS_ANTENNA_STATUS_NOT_DEPLOYED    1       /**< Value if antennas are not deployed yet */
#define ISIS_ANTENNA_STATUS_DEPLOYED        0       /**< Value if antennas are deployed         */

// Antenna stop cause
#define ISIS_ANTENNA_TIMEOUT_CAUSE          1       /**< Value if deployment system stops because timeout                   */
#define ISIS_ANTENNA_OTHER_CAUSE            0       /**< Value if deployment system stops because other reason than timeout */

// Antenna burn system
#define ISIS_ANTENNA_BURN_ACTIVE            1       /**< Value if the referring antenna burn system is active */
#define ISIS_ANTENNA_BURN_INACTIVE          0       /**< Value if the referring antenna burn system is off    */

#define ISIS_ANTENNA_ANT_1                  1       /**< . */
#define ISIS_ANTENNA_ANT_2                  2       /**< . */
#define ISIS_ANTENNA_ANT_3                  3       /**< . */
#define ISIS_ANTENNA_ANT_4                  4       /**< . */

#define VERIFY_STATUS(status, bit)          ( (status & bit) > 0 )

/**
 * \brief .
 *
 * \return None.
 */
void isis_antenna_init();

/**
 * \brief Verifies if the antenna is released or not.
 *
 * \return Deployment status. It can return:
 *              - true if the antenna is released.
 *              - false if the antenna is not released.
 *              .
 */
bool isis_antenna_is_released();

/**
 * \brief Enables the antenna deployment.
 *
 * \return It can be:
 *              - true
 *              - false
 *              .
 */
bool isis_antenna_release();

/**
 * \brief 
 *
 * \return None.
 */
void isis_antenna_arm();

/**
 * \brief 
 *
 * \return None.
 */
void isis_antenna_disarm();

/**
 * \brief 
 *
 * \param sec
 *
 * \return None.
 */
void isis_antenna_start_sequential_deploy(uint8_t sec);

/**
 * \brief 
 *
 * \param ant
 * \param sec
 * \param ovr
 *
 * \return None.
 */
void isis_antenna_start_independent_deploy(uint8_t ant, uint8_t sec, uint8_t ovr);

/**
 * \brief 
 *
 * \return The deployment status code.
 */
static uint16_t isis_antenna_read_deployment_status();

/**
 * \brief I2C interface initialization.
 *
 * \return None
 */
void isis_antenna_i2c_init();

/**
 * \brief Writes a byte in the I2C bus.
 *
 * \param byte is the byte to write in the I2C bus.
 *
 * \return None
 */
void isis_antenna_i2c_write_byte(uint8_t byte);

/**
 * \brief Writes data in the I2C bus.
 *
 * \param data is an array of data to write in the I2C bus.
 * \param len is the length of the data.
 *
 * \return None
 */
void isis_antenna_i2c_write_data(uint8_t *data, uint8_t len);

/**
 * \brief Read a byte from the I2C bus.
 *
 * \return The byte from the I2C bus.
 */
uint8_t isis_antenna_i2c_read_byte();

/**
 * \brief Read n bytes from the I2C bus.
 *
 * \param data is a pointe to write the data from the I2C bus.
 * \param len is the length of the data.
 *
 * \return None.
 */
void isis_antenna_i2c_read_data(uint8_t *data, uint8_t len);

/**
 * \brief Seconds delay.
 *
 * \param s is the delay in seconds.
 *
 * \return None
 */
void isis_antenna_delay_s(uint8_t s);

/**
 * \brief Milliseconds delay.
 *
 * \param ms is the delay in milliseconds.
 *
 * \return None
 */
void isis_antenna_delay_ms(uint16_t ms);

/**
 * \brief Microseconds delay
 *
 * \param us is the delay in microseconds.
 *
 * \return None
 */
void isis_antenna_delay_us(uint32_t us);

#endif // ANTENNA_H_

//! \} End of antenna group
