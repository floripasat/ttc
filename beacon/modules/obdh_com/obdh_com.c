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

#include "obdh_com.h"

OBDH *obdh_ptr;

uint8_t obdh_com_init(OBDH *obdh)
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("OBDH communication initialization... ");
#endif // BEACON_MODE
    
    obdh_ptr = obdh;
    
    // obdh_com initialization
    obdh->received_byte = OBDH_COM_DEFAULT_DATA_BYTE;
    obdh->byte_counter  = OBDH_COM_CMD_POSITION;
    obdh->crc_fails     = 0xFF;
    obdh->is_open       = false;
    
    obdh_com_clear_buffer(obdh);
    
    // obdh_data initialization
    obdh_com_save_data_from_buffer(obdh);
    
    if (obdh_com_spi_init() == STATUS_FAIL)
    {
        obdh->is_open = false;
        obdh->is_dead = true;
        
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("FAIL!\n");
#endif // BEACON_MODE

        return STATUS_FAIL;
    }
    else
    {
        obdh->is_open = true;
        obdh->is_dead = false;
        
        obdh_com_timer_timeout_init();
        
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("SUCCESS!\n");
#endif // BEACON_MODE

        return STATUS_SUCCESS;
    }
}

static uint8_t obdh_com_spi_init()
{
    // SPI pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(OBDH_COM_SPI_PORT, OBDH_COM_SPI_MOSI_PIN + OBDH_COM_SPI_MISO_PIN + OBDH_COM_SPI_SCLK_PIN + OBDH_COM_SPI_NSEL_PIN);
    
    if (USCI_A_SPI_initSlave(OBDH_COM_SPI_BASE_ADDRESS, USCI_A_SPI_MSB_FIRST,
                             USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
                             USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW) == STATUS_SUCCESS)
    {
        // Set SPI Mode 2 (The initSlave function from DriverLib, only initializes in Mode 0)
        HWREG8(OBDH_COM_SPI_BASE_ADDRESS + OFS_UCAxCTL0) |= UCMODE_2;
        
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

static void obdh_com_receive_data(OBDH *obdh)
{
    obdh->received_byte = USCI_A_SPI_receiveData(OBDH_COM_SPI_BASE_ADDRESS);
    
    switch(obdh->byte_counter)
    {
        case OBDH_COM_CMD_POSITION:
            obdh_com_receive_cmd(obdh);
            break;
        case OBDH_COM_SOD_POSITION:
            if (obdh->received_byte == OBDH_COM_START_OF_DATA)
            {
                obdh->byte_counter++;
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("ERROR! Invalid SOD byte received!\n");
#endif // DEBUG_MODE
                obdh->byte_counter = OBDH_COM_CMD_POSITION;
            }
            break;
        case OBDH_COM_CRC_POSITION:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("Checking CRC... ");
#endif // DEBUG_MODE
            if (obdh->received_byte == crc8(OBDH_COM_CRC_INITIAL_VALUE, OBDH_COM_CRC_POLYNOMIAL, obdh->buffer, OBDH_COM_DATA_PKT_LEN))
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("VALID!\n");
#endif // DEBUG_MODE
                obdh_com_save_data_from_buffer(obdh);
                
                obdh->crc_fails = 0;
                obdh->is_dead = false;
                
                obdh_com_timer_timeout_init();
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("ERROR! INVALID!\n");
#endif // DEBUG_MODE
                obdh_com_clear_buffer(obdh);
                if (obdh->crc_fails == 0xFF)
                {
                    obdh->crc_fails = 1;
                }
                else
                {
                    obdh->crc_fails++;
                }
            }
            obdh->byte_counter = OBDH_COM_CMD_POSITION;
            break;
        default:
            if ((obdh->byte_counter > OBDH_COM_SOD_POSITION) && (obdh->byte_counter < OBDH_COM_CRC_POSITION))
            {
                obdh_com_receive_pkt(obdh);
            }
            else
            {
                obdh->byte_counter = OBDH_COM_CMD_POSITION;
            }
            break;
    }
}

static void obdh_com_receive_cmd(OBDH *obdh)
{
    switch(obdh->received_byte)
    {
        case OBDH_COM_CMD_START_OF_DATA_TRANSFER:
            obdh->byte_counter = OBDH_COM_SOD_POSITION;
            break;
        case OBDH_COM_CMD_SHUTDOWN_REQUEST:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("Shutdown command received!\n");
#endif // DEBUG_MODE
            obdh_com_send_data(OBDH_COM_SHUTDOWN_ACK);
            if (beacon.flags.hibernation == false)
            {
                task_enter_hibernation();
            }
            break;
        case OBDH_COM_CMD_RF_MUTEX:
#if BEACON_MODE == DEBUG_MODE
            debug_print_msg("RF mutex command received!\n");
#endif // DEBUG_MODE
            if (beacon.flags.transmitting == true)
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("The Beacon is transmitting now! The transceiver must wait!\n");
#endif // DEBUG_MODE
                obdh_com_send_data(OBDH_COM_BEACON_BUSY);
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("The Beacon is not transmitting! The transceiver can transmit now!\n");
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

static void obdh_com_receive_pkt(OBDH *obdh)
{
    obdh->buffer[obdh->byte_counter - OBDH_COM_BAT1_VOLTAGE_POS] = obdh->received_byte;
    obdh->byte_counter++;
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

static void obdh_com_save_data_from_buffer(OBDH *obdh)
{
    uint8_t i = 0;
    uint8_t j = 0;
    
    for(i=0; i<OBDH_COM_BAT1_VOLTAGE_LEN; i++)
    {
        obdh->data.bat1_voltage[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_BAT2_VOLTAGE_LEN; i++)
    {
        obdh->data.bat2_voltage[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_BAT1_TEMPERATURE_LEN; i++)
    {
        obdh->data.bat1_temperature[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_BAT2_TEMPERATURE_LEN; i++)
    {
        obdh->data.bat2_temperature[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_BAT_CHARGE_LEN; i++)
    {
        obdh->data.bat_charge[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
    {
        obdh->data.solar_panels_currents[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
    {
        obdh->data.solar_panels_voltages[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_SAT_STATUS_LEN; i++)
    {
        obdh->data.sat_status[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_IMU_LEN; i++)
    {
        obdh->data.imu[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_SYSTEM_TIME_LEN; i++)
    {
        obdh->data.system_time[i] = obdh->buffer[j++];
    }
    
    for(i=0; i<OBDH_COM_RESET_COUNTER_LEN; i++)
    {
        obdh->data.reset_counter[i] = obdh->buffer[j++];
    }
}

static void obdh_com_clear_buffer(OBDH *obdh)
{
    memset(obdh->buffer, OBDH_COM_DEFAULT_DATA_BYTE, OBDH_COM_DATA_PKT_LEN*sizeof(uint8_t));
}

static void obdh_com_timer_timeout_init()
{
    Timer_B_clearTimerInterrupt(TIMER_B0_BASE);
    
    Timer_B_initContinuousModeParam param = {0};
    param.clockSource               = TIMER_B_CLOCKSOURCE_ACLK;
    param.clockSourceDivider        = TIMER_B_CLOCKSOURCE_DIVIDER_16;   // ~= 64 s to overflow
    param.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_ENABLE;
    param.timerClear                = TIMER_B_DO_CLEAR;
    param.startTimer                = false;
    
    Timer_B_initContinuousMode(TIMER_B0_BASE, &param);
    
    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_CONTINUOUS_MODE);
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
            obdh_com_receive_data(obdh_ptr);
            break;
        default:
            break;
    }
}

/**
 * \fn TIMERB1_ISR
 * 
 * \brief Timer_B7 Interrupt Vector (TBIV) handler.
 * 
 * \return None
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER0_B1_VECTOR)))
#endif
void TIMERB1_ISR()
{
    // Any access, read or write, of the TBIV register automatically resets the highest "pending" interrupt flag.
    switch(__even_in_range(TBIV,14))
    {
        case  0: break;
        case  2: break;
        case  4: break;
        case  6: break;
        case  8: break;
        case 10: break;
        case 12: break;
        case 14:            // Overflow
            obdh_ptr->is_dead = true;
            Timer_B_stop(TIMER_B0_BASE);
            Timer_B_clear(TIMER_B0_BASE);
        break;
        default: break;
    }
}

//! \} End of obdh_com group
