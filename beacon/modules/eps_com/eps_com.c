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

#include <config/config.h>
#include <modules/debug/debug.h>
#include <libs/crc/crc.h>

#include "eps_com.h"

EPSCom eps_com;

eps_com.eps_uart_received_byte                  = 0x00;
eps_com.eps_uart_byte_counter                   = 0x00;
eps_com.eps_data_buffer[EPS_UART_PKT_LEN + 1]   = {EPS_UART_DEFAULT_DATA_MSB, EPS_UART_DEFAULT_DATA_LSB, EPS_UART_DEFAULT_DATA_MSB, EPS_UART_DEFAULT_DATA_LSB};
eps_com.eps_data[EPS_UART_PKT_LEN + 1]          = {EPS_UART_DEFAULT_DATA_MSB, EPS_UART_DEFAULT_DATA_LSB, EPS_UART_DEFAULT_DATA_MSB, EPS_UART_DEFAULT_DATA_LSB};

EPSData eps_data;

uint8_t eps_com_init()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("EPS communication initialization... ");
#endif // DEBUG_MODE

    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(EPS_COM_UART_RX_PORT, EPS_COM_UART_RX_PIN);
    
    // Config UART (9600 bps, no parity, 1 stop bit, LSB first)
    USCI_A_UART_initParam uart_params = {0};
    uart_params.selectClockSource   = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uart_params.clockPrescalar      = 6;		// Clock = 1 MHz, Baudrate = 9600 bps ([1] http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html)
    uart_params.firstModReg         = 8;		// Clock = 1 MHz, Baudrate = 9600 bps (See [1])
    uart_params.secondModReg        = 0;		// Clock = 1 MHz, Baudrate = 9600 bps (See [1])
    uart_params.parity              = USCI_A_UART_NO_PARITY;
    uart_params.msborLsbFirst       = USCI_A_UART_LSB_FIRST;
    uart_params.numberofStopBits    = USCI_A_UART_ONE_STOP_BIT;
    uart_params.uartMode            = USCI_A_UART_MODE;
    uart_params.overSampling        = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;     // Clock = 1 MHz, Baudrate = 9600 bps (See [1])

    // UART initialization
    if (USCI_A_UART_init(EPS_COM_UART_BASE_ADDRESS, &uart_params) == STATUS_FAIL)
    {
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
        
        __enable_interrupt();

#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("SUCCESS!\n");
#endif // DEBUG_MODE

        return STATUS_SUCCESS;
    }
}

static void eps_com_receive_data()
{
#if BEACON_MODE == DEBUG_MODE
        debug_print_msg("Receiving a byte from the EPS module... ");
#endif // DEBUG_MODE

    eps_com.received_byte = USCI_A_UART_receiveData(EPS_COM_UART_BASE_ADDRESS);

    switch(eps_com.byte_counter)
    {
        case EPS_COM_BYTE_COUNTER_POS_SOD:
            if (eps_com.received_byte == EPS_COM_SOD)
            {
                eps_com.byte_counter++;
                
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("SOD byte received!\n");
#endif // DEBUG_MODE
            }
#if BEACON_MODE == DEBUG_MODE
            else
            {
                debug_print_msg("ERROR: Invalid SOD byte!\n");
            }
#endif // DEBUG_MODE
            break;
        case EPS_COM_BYTE_COUNTER_POS_CRC:
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("Checking CRC... ");
#endif // DEBUG_MODE
            if (eps_com.received_byte == crc8(EPS_COM_CRC_INITIAL_VALUE, EPS_COM_CRC_POLY, eps_data_buffer, sizeof(eps_data_buffer)-1))
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("Valid packet!\n");
#endif // DEBUG_MODE
                eps_data.bat1_msb = eps_com.buffer[0];
                eps_data.bat1_lsb = eps_com.buffer[1];
                eps_data.bat2_msb = eps_com.buffer[2];
                eps_data.bat2_lsb = eps_com.buffer[3];
            }
            else
            {
#if BEACON_MODE == DEBUG_MODE
                debug_print_msg("ERROR: Invalid packet!\n");
#endif // DEBUG_MODE
                eps_data.bat1_msb = EPS_COM_DEFAULT_DATA_MSB;
                eps_data.bat1_lsb = EPS_COM_DEFAULT_DATA_LSB;
                eps_data.bat2_msb = EPS_COM_DEFAULT_DATA_MSB;
                eps_data.bat2_lsb = EPS_COM_DEFAULT_DATA_LSB;
            }
        default:
            if ((eps_com.byte_counter > EPS_COM_BYTE_COUNTER_POS_SOD) &&
                (eps_com.byte_counter < EPS_COM_BYTE_COUNTER_POS_CRC))
            {
                eps_com.buffer[eps_com.byte_counter-1] = eps_com.received_byte;
                eps_com.byte_counter++;
            }
            else
            {
                eps_com.byte_counter = EPS_COM_BYTE_COUNTER_POS_SOD;
            }
    }
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
            eps_com_receive_data();
            break;
        default:
            break;
    }
}

//! \} End of eps_com group
