/*
 * uart-eps.c
 * 
 * Copyright (C) 2016, Universidade Federal de Santa Catarina
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
 * \file uart-eps.c
 * 
 * \brief EPS-UART functions implementation (EPS->TTC communication)
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \addtogroup eps_uart
 * \{
 */

#include "../inc/uart-eps.h"
#include "../inc/debug.h"
#include "../inc/crc.h"

// UART-EPS interruption variables initialization
uint8_t eps_uart_received_byte                  = 0x00;
uint8_t eps_uart_byte_counter                   = 0x00;
uint8_t eps_data_buffer[EPS_UART_PKT_LEN + 1]   = {EPS_UART_DEFAULT_DATA, EPS_UART_DEFAULT_DATA, EPS_UART_DEFAULT_DATA, EPS_UART_DEFAULT_DATA};
uint8_t eps_data[EPS_UART_PKT_LEN + 1]          = {EPS_UART_DEFAULT_DATA, EPS_UART_DEFAULT_DATA, EPS_UART_DEFAULT_DATA, EPS_UART_DEFAULT_DATA};

uint8_t eps_UART_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("EPS-UART initialization... ");
#endif // DEBUG_MODE

    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(EPS_UART_PORT, EPS_UART_RX_PIN);
    
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
    if (USCI_A_UART_init(EPS_UART_BASE_ADDRESS, &uart_params) == STATUS_FAIL)
    {
#if DEBUG_MODE == true
        debug_PrintMsg("FAIL!\n");
#endif // DEBUG_MODE
        
        return STATUS_FAIL;
    }
    else
    {
        // Enable UART module
        USCI_A_UART_enable(EPS_UART_BASE_ADDRESS);
        
        // Enable Receive Interrupt
        USCI_A_UART_clearInterrupt(EPS_UART_BASE_ADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);
        USCI_A_UART_enableInterrupt(EPS_UART_BASE_ADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);
        
        __enable_interrupt();

#if DEBUG_MODE == true
        debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE

        return STATUS_SUCCESS;
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
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR()
{
    switch(__even_in_range(UCA0IV, 4))
    {
        // Vector 2 - RXIFG
        case 2:
            eps_uart_received_byte = USCI_A_UART_receiveData(USCI_A0_BASE);

            switch(eps_uart_byte_counter)
            {
                case EPS_UART_BYTE_COUNTER_POS_SOD:
                    if (eps_uart_received_byte == EPS_UART_SOD)
                    {
                        eps_uart_byte_counter++;
                    }
                    break;
                case EPS_UART_BYTE_COUNTER_POS_CRC:
                    if (eps_uart_received_byte == crc8(EPS_UART_CRC_INITIAL_VALUE, EPS_UART_CRC_POLY, eps_data_buffer, sizeof(eps_data_buffer)-1))
                    {
                        uint8_t i = 0;
                        for(i=0; i<sizeof(eps_data_buffer); i++)
                        {
                            eps_data[i] = eps_data_buffer[i];
                        }
                    }
                    else
                    {
                        uint8_t i = 0;
                        for(i=0; i<sizeof(eps_data); i++)
                        {
                            eps_data[i] = EPS_UART_DEFAULT_DATA;
                        }
                    }
                default:
                    if ((eps_uart_byte_counter > EPS_UART_BYTE_COUNTER_POS_SOD) &&
                        (eps_uart_byte_counter < EPS_UART_BYTE_COUNTER_POS_CRC))
                    {
                        eps_data_buffer[eps_uart_byte_counter-1] = eps_uart_received_byte;
                        eps_uart_byte_counter++;
                    }
                    else
                    {
                        eps_uart_byte_counter = EPS_UART_BYTE_COUNTER_POS_SOD;
                    }
            };
            break;
        default:
            break;
    }
}

//! \} End of eps_uart implementation group
