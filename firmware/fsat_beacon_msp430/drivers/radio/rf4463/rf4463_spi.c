/*
 * rf4463_spi.c
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
 * \brief NiceRF RF4463 driver (SPI communication functions implementation).
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.12
 * 
 * \date 29/07/2017
 * 
 * \defgroup rf4463_spi SPI
 * \ingroup rf4463
 * \{
 */

#include <stdint.h>
#include <drivers/driverlib/driverlib.h>
#include <system/debug/debug.h>

#include "rf4463_spi.h"
#include "rf4463_pinmap.h"
#include "rf4463_config.h"
#include "rf4463_registers.h"

uint8_t rf4463_spi_init()
{
    debug_print_event_from_module(DEBUG_INFO, RF4463_MODULE_NAME, "Configuring the GPIO pins...\n\r");

    // MISO, MOSI and SCLK init.
    GPIO_setAsPeripheralModuleFunctionInputPin(RF4463_SPI_PORT, RF4463_SDI_PIN + RF4463_SDO_PIN + RF4463_SCLK_PIN);

    // CSn init.
    GPIO_setAsOutputPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
	GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);

    // Config. SPI as Master
#if RF4463_SPI_USCI == USCI_A
    USCI_A_SPI_initMasterParam spi_params = {0};
    spi_params.selectClockSource     = USCI_A_SPI_CLOCKSOURCE_SMCLK;
    spi_params.msbFirst              = USCI_A_SPI_MSB_FIRST;
    spi_params.clockPhase            = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    spi_params.clockPolarity         = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
#elif RF4463_SPI_USCI == USCI_B
    USCI_B_SPI_initMasterParam spi_params = {0};
    spi_params.selectClockSource     = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    spi_params.msbFirst              = USCI_B_SPI_MSB_FIRST;
    spi_params.clockPhase            = USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    spi_params.clockPolarity         = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
#endif // RF4463_SPI_USCI
    spi_params.clockSourceFrequency  = UCS_getSMCLK();
    spi_params.desiredSpiClock       = RF4463_SPI_CLK;

    // SPI initialization
#if RF4463_SPI_USCI == USCI_A
    if (USCI_A_SPI_initMaster(RF4463_SPI_BASE_ADDRESS, &spi_params) == STATUS_SUCCESS)
    {
        USCI_A_SPI_enable(RF4463_SPI_BASE_ADDRESS);
#elif RF4463_SPI_USCI == USCI_B
    if (USCI_B_SPI_initMaster(RF4463_SPI_BASE_ADDRESS, &spi_params) == STATUS_SUCCESS)
    {
        USCI_B_SPI_enable(RF4463_SPI_BASE_ADDRESS);
#endif // RF4463_SPI_USCI

        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

static void rf4463_spi_write_byte(uint8_t byte)
{
#if RF4463_SPI_USCI == USCI_A
    // Wait until TX buffer ready
    while(!USCI_A_SPI_getInterruptStatus(RF4463_SPI_BASE_ADDRESS, USCI_A_SPI_TRANSMIT_INTERRUPT))
    {

    }
    USCI_A_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_A_SPI_TRANSMIT_INTERRUPT);
    
    USCI_A_SPI_transmitData(RF4463_SPI_BASE_ADDRESS, byte);
#elif RF4463_SPI_USCI == USCI_B
    // Wait until TX buffer ready
    while(!USCI_B_SPI_getInterruptStatus(RF4463_SPI_BASE_ADDRESS, USCI_B_SPI_TRANSMIT_INTERRUPT))
    {

    }
    USCI_B_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_B_SPI_TRANSMIT_INTERRUPT);
    
    USCI_B_SPI_transmitData(RF4463_SPI_BASE_ADDRESS, byte);
#endif // RF4463_SPI_USCI
}

void rf4463_spi_write(uint8_t *data, uint16_t size)
{
    while(size--)
    {
        rf4463_spi_transfer(*data++);
    }
}

static uint8_t rf4463_spi_read_byte()
{    
#if RF4463_SPI_USCI == USCI_A
    // Wait until new data was written into RX buffer
    while(!USCI_A_SPI_getInterruptStatus(RF4463_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
    {
        
    }
    USCI_A_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);

    return USCI_A_SPI_receiveData(RF4463_SPI_BASE_ADDRESS);
#elif RF4463_SPI_USCI == USCI_B
    // Wait until new data was written into RX buffer
    while(!USCI_B_SPI_getInterruptStatus(RF4463_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
    {
        
    }
    USCI_B_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);

    return USCI_B_SPI_receiveData(RF4463_SPI_BASE_ADDRESS);
#endif // RF4463_SPI_USCI
}

void rf4463_spi_read(uint8_t *data, uint16_t size)
{
    while(size--)
    {
        *data++ = rf4463_spi_transfer(RF4463_CMD_NOP);
    }
}

uint8_t rf4463_spi_transfer(uint8_t byte)
{
    rf4463_spi_write_byte(byte);
    return rf4463_spi_read_byte();
}

//! \} End of rf4463_spi group
