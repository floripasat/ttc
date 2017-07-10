/*
 * obdh_com.c
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
 * \file obdh_com.c
 * 
 * \brief Implementation of the functions to communicate with the OBDH module.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \addtogroup obdh_com
 * \{
 */

#include <stdbool.h>
#include <string.h>

#include <config/config.h>
#include <modules/modules.h>
#include <libs/driverlib/driverlib.h>
#include <libs/crc/crc.h>
#include <src/tasks.h>
#include <src/beacon.h>

#include "obdh_com.h"
#include "obdh_com_config.h"

OBDHCom obdh_com;

OBDHData obdh_data;

uint8_t obdh_com_init()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Initializing OBDH communication peripherals... ");
#endif // BEACON_MODE

    while(obdh_com_spi_init() != STATUS_SUCCESS)
    {
        
    }

    // obdh_com initialization
    obdh_com.received_byte          = 0x00;
    obdh_com.byte_counter           = 0x00;
    obdh_com_clear_buffer();
    
    // obdh_data initialization
    /*
    obdh_data.v_bat1[3]             = {0xFF};
    obdh_data.v_bat2[3]             = {0xFF};
    obdh_data.i_solar_panels[13]    = {0xFF};
    obdh_data.v_solar_panels[7]     = {0xFF};
    obdh_data.t_bats[7]             = {0xFF};
    obdh_data.imu[13]               = {0xFF};
    obdh_data.q_bats[3]             = {0xFF};
    obdh_data.system_time[5]        = {0xFF};
    obdh_data.sat_status[3]         = {0xFF};
    obdh_data.reset_counter[3]      = {0xFF};
    */

#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("SUCCESS!\n");
#endif // BEACON_MODE

    return STATUS_SUCCESS;
}

static uint8_t obdh_com_spi_init()
{
    // If clock signal from master stays low, it is not yet in SPI mode
    while(GPIO_getInputPinValue(OBDH_COM_SPI_PORT, OBDH_COM_SPI_SCLK_PIN) == GPIO_INPUT_PIN_LOW)
    {
        
    }
    
    // SPI pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(OBDH_COM_SPI_PORT, OBDH_COM_SPI_MOSI_PIN + OBDH_COM_SPI_MISO_PIN + OBDH_COM_SPI_SCLK_PIN);
    
    if (USCI_A_SPI_initSlave(OBDH_COM_SPI_BASE_ADDRESS, USCI_A_SPI_MSB_FIRST, USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT, USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH) == STATUS_SUCCESS)
    {
        // Enable SPI Module
        USCI_A_SPI_enable(OBDH_COM_SPI_BASE_ADDRESS);

        // Enable Receive interrupt
        USCI_A_SPI_clearInterrupt(OBDH_COM_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
        USCI_A_SPI_enableInterrupt(OBDH_COM_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
        
        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

static void obdh_com_receive_data()
{
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("Receiving a byte from the OBDH module... ");
#endif // DEBUG_MODE

    obdh_com.received_byte = USCI_A_SPI_receiveData(OBDH_COM_SPI_BASE_ADDRESS);
    
    switch(obdh_com.byte_counter)
    {
        case OBDH_COM_CMD_POSITION:
            obdh_com_receive_cmd(obdh_com.received_byte);
            break;
        case OBDH_COM_SOD_POSITION:
            if (obdh_com.received_byte == OBDH_COM_START_OF_DATA)
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("SOD byte received!\n");
#endif // DEBUG_MODE
                obdh_com.byte_counter++;
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("ERROR! Invalid SOD byte received!\nWaiting a new valid command...\n");
#endif // DEBUG_MODE
                obdh_com.byte_counter = OBDH_COM_CMD_POSITION;
            }
            break;
        case OBDH_COM_CRC_POSITION:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("Checking CRC... ");
#endif // DEBUG_MODE
            if (obdh_com.received_byte == crc8(OBDH_COM_CRC_INITIAL_VALUE, OBDH_COM_CRC_POLYNOMIAL, obdh_com.buffer, OBDH_COM_DATA_PKT_LEN))
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("VALID!\n");
#endif // DEBUG_MODE
                obdh_com_save_data_from_buffer();
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("INVALID!\n");
#endif // DEBUG_MODE
                obdh_com_clear_buffer();
            }
            break;
        default:
            if ((obdh_com.byte_counter > OBDH_COM_SOD_POSITION) && (obdh_com.byte_counter < OBDH_COM_CRC_POSITION))
            {
                obdh_com_receive_pkt(obdh_com.received_byte);
            }
            else
            {
                obdh_com.byte_counter = OBDH_COM_CMD_POSITION;
            }
            break;
    }
}

static void obdh_com_receive_cmd(uint8_t cmd)
{
    switch(cmd)
    {
        case OBDH_COM_CMD_START_OF_DATA_TRANSFER:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("Data transfer command received!\n");
#endif // DEBUG_MODE
            obdh_com.byte_counter = OBDH_COM_SOD_POSITION;
            break;
        case OBDH_COM_CMD_SHUTDOWN_REQUEST:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("Shutdown command received!\n");
#endif // DEBUG_MODE
            obdh_com_send_data(OBDH_COM_SHUTDOWN_ACK);
            task_enter_hibernation();
            break;
        case OBDH_COM_CMD_RF_MUTEX:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("RF mutex command received!\n");
#endif // DEBUG_MODE
            if (beacon.flags.transmitting == true)
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("All of our channels are busy now! (Elevator music starts)\n");
#endif // DEBUG_MODE
                obdh_com_send_data(OBDH_COM_BEACON_BUSY);
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("RUN, FORREST, RUN!\n");
#endif // DEBUG_MODE
                obdh_com_send_data(OBDH_COM_BEACON_FREE);
                beacon.flags.can_transmit = false;
            }
            break;
        case OBDH_COM_TRANSCEIVER_FREE:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("Beacon channel free command received!\n");
#endif // DEBUG_MODE
            beacon.flags.can_transmit = true;
            break;
        default:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("ERROR! Invalid command!\n");
#endif // DEBUG_MODE
            break;
    }
}

static void obdh_com_receive_pkt(uint8_t byte)
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Packet data byte received!\n");
#endif // DEBUG_MODE
    obdh_com.buffer[obdh_com.byte_counter - OBDH_COM_VBAT1_POS] = byte;
    obdh_com.byte_counter++;
}

void obdh_com_send_data(uint8_t data)
{
    // USCI_An TX buffer ready?
    while(!USCI_A_SPI_getInterruptStatus(OBDH_COM_SPI_BASE_ADDRESS, USCI_A_SPI_TRANSMIT_INTERRUPT))
    {
        
    }

    // Transmit data to master
    USCI_A_SPI_transmitData(OBDH_COM_SPI_BASE_ADDRESS, data);
}

static void obdh_com_save_data_from_buffer()
{
    uint8_t i = 0;
    uint8_t j = 0;
    
    obdh_data.v_bat1[0] = obdh_com.buffer[j++];
    obdh_data.v_bat1[1] = obdh_com.buffer[j++];
    
    obdh_data.v_bat2[0] = obdh_com.buffer[j++];
    obdh_data.v_bat2[1] = obdh_com.buffer[j++];
    
    for(i=0;i<OBDH_COM_I_SOLAR_PANELS_LEN;i++)
    {
        obdh_data.i_solar_panels[i] = obdh_com.buffer[j++];
    }
    
    for(i=0;i<OBDH_COM_V_SOLAR_PANELS_LEN;i++)
    {
        obdh_data.v_solar_panels[i] = obdh_com.buffer[j++];
    }
    
    for(i=0;i<OBDH_COM_TEMP_BATTS_LEN;i++)
    {
        obdh_data.t_bats[i] = obdh_com.buffer[j++];
    }
    
    for(i=0;i<OBDH_COM_IMU_LEN;i++)
    {
        obdh_data.imu[i] = obdh_com.buffer[j++];
    }
    
    obdh_data.q_bats[0] = obdh_com.buffer[j++];
    obdh_data.q_bats[1] = obdh_com.buffer[j++];
    
    for(i=0;i<OBDH_COM_SYSTEM_TIME_LEN;i++)
    {
        obdh_data.system_time[i] = obdh_com.buffer[j++];
    }
    
    obdh_data.sat_status[0] = obdh_com.buffer[j++];
    obdh_data.sat_status[1] = obdh_com.buffer[j++];
    
    obdh_data.reset_counter[0] = obdh_com.buffer[j++];
    obdh_data.reset_counter[1] = obdh_com.buffer[j++];
}

static void obdh_com_clear_buffer()
{
    memset(obdh_com.buffer, 0xFF, OBDH_COM_DATA_PKT_LEN*sizeof(uint8_t));
}

/**
 * \fn USCI_A2_IST
 * 
 * \brief USCI_A2 interrupt vector service routine.
 * 
 * \return None
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A2_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A2_VECTOR)))
#endif
void USCI_A2_ISR()
{
    switch(__even_in_range(UCA2IV, 4))
    {
        //Vector 2 - RXIFG
        case 2:
            obdh_com_receive_data();
            break;
        default:
            break;
    }
}

//! \} End of obdh_com group
