/*
 * isis_antenna_i2c.c
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
 * \file isis_antenna_i2c.c
 * 
 * \brief ISIS antenna driver I2C interface implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 21/09/2017
 * 
 * \addtogroup isis_antenna
 * \{
 */

#include <drivers/driverlib/driverlib.h>

#include "isis_antenna_i2c.h"
#include "isis_antenna_pinmap.h"
#include "isis_antenna_config.h"

void isis_antenna_i2c_init()
{
    GPIO_setAsPeripheralModuleFunctionInputPin(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SDA_PIN + ISIS_ANTENNA_I2C_SCL_PIN);
    
#if ISIS_ANTENNA_I2C_USCI == USCI_A
    USCI_A_I2C_initMasterParam i2c_params = {0};
    
    i2c_params.selectClockSource    = ISIS_ANTENNA_I2C_CLOCK_SOURCE;
    i2c_params.i2cClk               = ISIS_ANTENNA_I2C_CLOCK;
    i2c_params.dataRate             = ISIS_ANTENNA_I2C_DATA_RATE;
    
    USCI_A_I2C_initMaster(ISIS_ANTENNA_I2C_BASE_ADDRESS, &i2c_params);
    
    // Specify slave address
    USCI_A_I2C_setSlaveAddress(ISIS_ANTENNA_I2C_BASE_ADDRESS, ISIS_ANTENNA_I2C_SLAVE_ADDRESS);
    
    // Set in transmit mode
    USCI_A_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_A_I2C_TRANSMIT_MODE);
    
    // Enable I2C Module to start operations
    USCI_A_I2C_enable(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#elif ISIS_ANTENNA_I2C_USCI == USCI_B
    USCI_B_I2C_initMasterParam i2c_params = {0};
    
    i2c_params.selectClockSource    = ISIS_ANTENNA_I2C_CLOCK_SOURCE;
    i2c_params.i2cClk               = ISIS_ANTENNA_I2C_CLOCK;
    i2c_params.dataRate             = ISIS_ANTENNA_I2C_DATA_RATE;
    
    USCI_B_I2C_initMaster(ISIS_ANTENNA_I2C_BASE_ADDRESS, &i2c_params);
    
    // Specify slave address
    USCI_B_I2C_setSlaveAddress(ISIS_ANTENNA_I2C_BASE_ADDRESS, ISIS_ANTENNA_I2C_SLAVE_ADDRESS);
    
    // Set in transmit mode
    USCI_B_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_B_I2C_TRANSMIT_MODE);
    
    // Enable I2C Module to start operations
    USCI_B_I2C_enable(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#endif // ISIS_ANTENNA_I2C_USCI
}

void isis_antenna_i2c_write_byte(uint8_t byte)
{
#if ISIS_ANTENNA_I2C_USCI == USCI_A
    // Set in transmit mode
    USCI_A_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_A_I2C_TRANSMIT_MODE);
    
    // Send single byte data
    USCI_A_I2C_masterSendSingleByte(ISIS_ANTENNA_I2C_BASE_ADDRESS, byte);
    
    // Wait until transmission completes
    while(USCI_A_I2C_isBusBusy(ISIS_ANTENNA_I2C_BASE_ADDRESS))
    {
        
    }
#elif ISIS_ANTENNA_I2C_USCI == USCI_B
    // Set in transmit mode
    USCI_B_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_B_I2C_TRANSMIT_MODE);
    
    // Send single byte data
    USCI_B_I2C_masterSendSingleByte(ISIS_ANTENNA_I2C_BASE_ADDRESS, byte);
    
    // Wait until transmission completes
    while(USCI_B_I2C_isBusBusy(ISIS_ANTENNA_I2C_BASE_ADDRESS))
    {
        
    }
#endif // ISIS_ANTENNA_I2C_USCI
}

void isis_antenna_i2c_write_data(uint8_t *data, uint8_t len)
{
#if ISIS_ANTENNA_I2C_USCI == USCI_A
    // Set in transmit mode
    USCI_A_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_A_I2C_TRANSMIT_MODE);
    
    USCI_A_I2C_masterSendMultiByteStart(ISIS_ANTENNA_I2C_BASE_ADDRESS, data[0]);
    
    uint8_t i = 1;
    for(i=1; i<len; i++)
    {
        USCI_A_I2C_masterSendMultiByteNext(ISIS_ANTENNA_I2C_BASE_ADDRESS, data[i]);
    }
    
    USCI_A_I2C_masterSendMultiByteStop(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#elif ISIS_ANTENNA_I2C_USCI == USCI_B
    // Set in transmit mode
    USCI_B_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_B_I2C_TRANSMIT_MODE);
    
    USCI_B_I2C_masterSendMultiByteStart(ISIS_ANTENNA_I2C_BASE_ADDRESS, data[0]);
    
    uint8_t i = 1;
    for(i=1; i<len; i++)
    {
        USCI_B_I2C_masterSendMultiByteNext(ISIS_ANTENNA_I2C_BASE_ADDRESS, data[i]);
    }
    
    USCI_B_I2C_masterSendMultiByteStop(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#endif // ISIS_ANTENNA_I2C_USCI
}

uint8_t isis_antenna_i2c_read_byte()
{
#if ISIS_ANTENNA_I2C_USCI == USCI_A
    USCI_A_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_A_I2C_RECEIVE_MODE);
    
    return USCI_A_I2C_masterReceiveSingle(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#elif ISIS_ANTENNA_I2C_USCI == USCI_B
    USCI_B_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_B_I2C_RECEIVE_MODE);
    
    return USCI_B_I2C_masterReceiveSingle(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#endif // ISIS_ANTENNA_I2C_USCI
}

void isis_antenna_i2c_read_data(uint8_t *data, uint8_t len)
{
#if ISIS_ANTENNA_I2C_USCI == USCI_A
    USCI_A_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_A_I2C_RECEIVE_MODE);
    
    USCI_A_I2C_masterReceiveMultiByteStart(ISIS_ANTENNA_I2C_BASE_ADDRESS);
    
    uint8_t i = 0;
    for(i=0; i<len-1; i++)
    {
        data[i] = USCI_A_I2C_masterReceiveMultiByteNext(ISIS_ANTENNA_I2C_BASE_ADDRESS);
    }
    
    data[len-1] = USCI_A_I2C_masterReceiveMultiByteFinish(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#elif ISIS_ANTENNA_I2C_USCI == USCI_B
    USCI_B_I2C_setMode(ISIS_ANTENNA_I2C_BASE_ADDRESS, USCI_B_I2C_RECEIVE_MODE);
    
    USCI_B_I2C_masterReceiveMultiByteStart(ISIS_ANTENNA_I2C_BASE_ADDRESS);
    
    uint8_t i = 0;
    for(i=0; i<len-1; i++)
    {
        data[i] = USCI_B_I2C_masterReceiveMultiByteNext(ISIS_ANTENNA_I2C_BASE_ADDRESS);
    }
    
    data[len-1] = USCI_B_I2C_masterReceiveMultiByteFinish(ISIS_ANTENNA_I2C_BASE_ADDRESS);
#endif // ISIS_ANTENNA_I2C_USCI
}

//! \} End of isis_antenna group
