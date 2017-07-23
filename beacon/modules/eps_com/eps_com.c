/*
 * eps_com.c
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
 * \file eps_com.c
 * 
 * \brief EPS-UART functions implementation (EPS->TTC communication)
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \addtogroup eps_com
 * \{
 */

#include <string.h>

#include <config/config.h>
#include <modules/debug/debug.h>
#include <libs/libs.h>

#include "eps_com.h"

EPS *eps_ptr;

uint8_t eps_com_init(EPS *eps)
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("EPS communication initialization... ");
#endif // DEBUG_MODE

    eps_ptr = eps;

    // EPS initialization
    eps->received_byte  = EPS_COM_DEFAULT_DATA_BYTE;
    eps->byte_counter   = EPS_COM_PKT_SOD_POSITION;
    eps->crc_fails      = 0;
    eps->is_open        = false;
    
    eps_com_clear_buffer(eps);
    
    // EPS data initialization
    eps_com_save_data_from_buffer(eps);

    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(EPS_COM_UART_RX_PORT, EPS_COM_UART_RX_PIN);
    
    // Config UART (4800 bps, no parity, 1 stop bit, LSB first)
    USCI_A_UART_initParam uart_params = {0};
    uart_params.selectClockSource   = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uart_params.clockPrescalar      = 52;		// Clock = 4 MHz, Baudrate = 4800 bps ([1] http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html)
    uart_params.firstModReg         = 1;		// Clock = 4 MHz, Baudrate = 4800 bps (See [1])
    uart_params.secondModReg        = 0;		// Clock = 4 MHz, Baudrate = 4800 bps (See [1])
    uart_params.parity              = USCI_A_UART_NO_PARITY;
    uart_params.msborLsbFirst       = USCI_A_UART_LSB_FIRST;
    uart_params.numberofStopBits    = USCI_A_UART_ONE_STOP_BIT;
    uart_params.uartMode            = USCI_A_UART_MODE;
    uart_params.overSampling        = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;     // Clock = 1 MHz, Baudrate = 9600 bps (See [1])

    // UART initialization
    if (USCI_A_UART_init(EPS_COM_UART_BASE_ADDRESS, &uart_params) == STATUS_FAIL)
    {
        eps->is_open = false;
        
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("FAIL!\n");
#endif // DEBUG_MODE
        
        return STATUS_FAIL;
    }
    else
    {
        // Enable UART module
        USCI_A_UART_enable(EPS_COM_UART_BASE_ADDRESS);
        
        // Enable Receive Interrupt
        USCI_A_UART_clearInterrupt(EPS_COM_UART_BASE_ADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);
        USCI_A_UART_enableInterrupt(EPS_COM_UART_BASE_ADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);

        eps->is_open = true;

#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("SUCCESS!\n");
#endif // DEBUG_MODE

        return STATUS_SUCCESS;
    }
}

static void eps_com_receive_data(EPS *eps)
{
    eps->received_byte = USCI_A_UART_receiveData(EPS_COM_UART_BASE_ADDRESS);

    switch(eps->byte_counter)
    {
        case EPS_COM_PKT_SOD_POSITION:
            if (eps->received_byte == EPS_COM_PKT_SOD)
            {
                eps->byte_counter++;
            }
#if BEACON_MODE == DEBUG_MODE
            else
            {
                debug_print_msg("ERROR: Invalid SOD byte received from the EPS module!\n");
            }
#endif // DEBUG_MODE
            break;
        case EPS_COM_CRC_POSITION:
            if (eps->received_byte == crc8(EPS_COM_CRC_INITIAL_VALUE, EPS_COM_CRC_POLYNOMIAL, eps->buffer, sizeof(eps->buffer)-1))
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("Data was received from the EPS module!\n");
#endif // DEBUG_MODE
                eps_com_save_data_from_buffer(eps);
                eps->crc_fails = 0;
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("ERROR! Invalid data received from the EPS module!\n");
#endif // DEBUG_MODE
                eps_com_clear_buffer(eps);
                eps->crc_fails++;
            }
            eps->byte_counter = EPS_COM_PKT_SOD_POSITION;
            break;
        default:
            if ((eps->byte_counter > EPS_COM_PKT_SOD_POSITION) &&
                (eps->byte_counter < EPS_COM_CRC_POSITION))
            {
                eps->buffer[eps->byte_counter-1] = eps->received_byte;
                eps->byte_counter++;
            }
            else
            {
                eps->byte_counter = EPS_COM_PKT_SOD_POSITION;
            }
            break;
    }
}

static void eps_com_save_data_from_buffer(EPS *eps)
{
    uint8_t i = 0;
    uint8_t j = 0;
    
    for(i=0; i<EPS_COM_BAT1_VOLTAGE_LEN; i++)
    {
        eps->data.bat1_voltage[i] = eps->buffer[j++];
    }
    
    for(i=0; i<EPS_COM_BAT2_VOLTAGE_LEN; i++)
    {
        eps->data.bat2_voltage[i] = eps->buffer[j++];
    }
    
    for(i=0; i<EPS_COM_BAT1_TEMPERATURE_LEN; i++)
    {
        eps->data.bat1_temperature[i] = eps->buffer[j++];
    }
    
    for(i=0; i<EPS_COM_BAT2_TEMPERATURE_LEN; i++)
    {
        eps->data.bat2_temperature[i] = eps->buffer[j++];
    }
    
    for(i=0; i<EPS_COM_BAT_CHARGE_LEN; i++)
    {
        eps->data.bat_charge[i] = eps->buffer[j++];
    }
    
    for(i=0; i<EPS_COM_SOLAR_PANELS_CURRENTS_LEN; i++)
    {
        eps->data.solar_panels_currents[i] = eps->buffer[j++];
    }
    
    for(i=0; i<EPS_COM_SOLAR_PANELS_VOLTAGES_LEN; i++)
    {
        eps->data.solar_panels_voltages[i] = eps->buffer[j++];
    }
    
    eps->data.energy_level = eps->buffer[j++];
}

static void eps_com_clear_buffer(EPS *eps)
{
    memset(eps->buffer, EPS_COM_DEFAULT_DATA_BYTE, EPS_COM_DATA_PKT_LEN*sizeof(uint8_t));
}

/**
 * \fn USCI_A0_ISR
 *
 * \brief This is the USCI_A0 interrupt vector service routine.
 *
 * UART RX interruption routine.
 *
 * \return none
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=EPS_COM_UART_ISR_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(EPS_COM_UART_ISR_VECTOR)))
#endif
void USCI_A0_ISR()
{
    switch(__even_in_range(UCA0IV, 4))
    {
        // Vector 2 - RXIFG
        case 2:
            eps_com_receive_data(eps_ptr);
            break;
        default:
            break;
    }
}

//! \} End of eps_com group
