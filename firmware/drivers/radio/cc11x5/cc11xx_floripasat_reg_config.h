/*
 * cc11xx_floripasat_reg_config.h
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina.
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 *
 */ 

/**
 * \brief CC1125/CC1175 registers values to set
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup cc11xx_reg_config Register Values
 * \ingroup cc11xx
 * \{
 */

#ifndef CC11XX_FLORIPASAT_REG_CONFIG_H_
#define CC11XX_FLORIPASAT_REG_CONFIG_H_

#include <stdint.h>

#include "cc11xx.h"

/**
 * \brief CC11XX register setting basic struct
 * 
 * This struct contains the address and the value of a register.
 */
typedef struct
{
    uint16_t addr;
    uint8_t  data;
} RegistersSettings;

/**
 * Registers values for:
 *      - Carrier frequency    = 437,5 MHz
 *      - XTAL frequency       = 32 MHz
 *      - Symbol rate          = 1,2 ksps
 *      - Deviation            = 3,997803 (~= 4)
 *      - Modulation           = 2-GFSK
 *      - RX filter bandwidth  = 12,5 kHz
 *      - TX power             = 10 dBm
 *      .
 * 
 * All values obtained from SmartRF Studio.
 */
static const RegistersSettings reg_values[] = 
{
    {CC11XX_IOCFG3,             0xB0},
    {CC11XX_IOCFG2,             0x06},
    {CC11XX_IOCFG1,             0xB0},
    {CC11XX_IOCFG0,             0x40},
    {CC11XX_SYNC3,              0x04},      // Sync word [31:24]
    {CC11XX_SYNC2,              0x08},      // Sync word [23:16]
    {CC11XX_SYNC1,              0x0F},      // Sync word [15:8]
    {CC11XX_SYNC0,              0x10},      // Sync word [7:0]
    {CC11XX_SYNC_CFG1,          0x0B},
    {CC11XX_SYNC_CFG0,          0x17},      // Bit error qualifier disabled. No check on bit errors; 32 bits sync word 
    {CC11XX_MODCFG_DEV_E,       0x0B},      // MOD_FORMAT = 001 (2-GFSK)
    {CC11XX_DCFILT_CFG,         0x1C},
    {CC11XX_PREAMBLE_CFG1,      0x18},      // PREAMBLE_WORD = 00 (Preamble = 10101010 = 0xAA), NUM_PREAMBLE = 0110 (4 bytes)
    {CC11XX_IQIC,               0xC6},
    {CC11XX_CHAN_BW,            0x10},
    {CC11XX_MDMCFG1,            0x40},      // FIFO_EN = 1, MANCHESTER_EN = 0
    {CC11XX_MDMCFG0,            0x05},      // TRANSPARENT_MODE_EN = 0
    {CC11XX_AGC_REF,            0x20},
    {CC11XX_AGC_CS_THR,         0x19},
    {CC11XX_AGC_CFG1,           0xA9},
    {CC11XX_AGC_CFG0,           0xCF},
    {CC11XX_FIFO_CFG,           0x00},
    {CC11XX_FS_CFG,             0x14},
    {CC11XX_PKT_CFG2,           0x00},      // PKT_FORMAT = 00 (FIFO mode)
    {CC11XX_PKT_CFG1,           0x00},      // CRC disabled for TX and RX
    {CC11XX_PKT_CFG0,           0x00},
    {CC11XX_PA_CFG0,            0x7E},
    {CC11XX_PKT_LEN,            0x1E},      // AX25 header + "FLORIPASAT" = 0x1E
    {CC11XX_IF_MIX_CFG,         0x00},
    {CC11XX_FREQOFF_CFG,        0x22},
    {CC11XX_FREQ2,              0x6D},
    {CC11XX_FREQ1,              0x60},
    {CC11XX_FS_DIG1,            0x00},
    {CC11XX_FS_DIG0,            0x5F},
    {CC11XX_FS_CAL0,            0x0E},
    {CC11XX_FS_DIVTWO,          0x03},
    {CC11XX_FS_DSM0,            0x33},
    {CC11XX_FS_DVC0,            0x17},
    {CC11XX_FS_PFD,             0x50},
    {CC11XX_FS_PRE,             0x6E},
    {CC11XX_FS_REG_DIV_CML,     0x14},
    {CC11XX_FS_SPARE,           0xAC},
    {CC11XX_XOSC5,              0x0E},
    {CC11XX_XOSC3,              0xC7},
    {CC11XX_XOSC1,              0x07}
};

/**
 * \brief Register settings for activating the internal temperature sensor.
 * 
 * \see CC112x/CC1175 User's Guide, table 12.
 */
static const RegistersSettings temp_sensor_values[] =
{
    {CC11XX_IOCFG1,             0x80},
    {CC11XX_ATEST,              0x2A},
    {CC11XX_ATEST_MODE,         0x0C},
    {CC11XX_GBIAS1,             0x07}
};

#endif // CC11XX_FLORIPASAT_REG_CONFIG_H_

//! \} End of CC1175 registers configuration group
