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
#include "../driverlib/driverlib.h"

uint8_t eps_UART_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("UART_EPS_Init()");
#endif // DEBUG_MODE

    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(EPS_UART_PORT, GPIO_PIN5);
    
    // Config UART (115200 bps, no parity, 1 stop bit, LSB first)
    USCI_A_UART_initParam uart_params = {0};
    uart_params.selectClockSource   = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uart_params.clockPrescalar      = 8;		// Clock = 1 MHz, Baudrate = 115200 bps	([1] http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html)
    uart_params.firstModReg         = 0;		// Clock = 1 MHz, Baudrate = 115200 bps (See [1])
    uart_params.secondModReg        = 6;		// Clock = 1 MHz, Baudrate = 115200 bps (See [1])
    uart_params.parity              = USCI_A_UART_NO_PARITY;
    uart_params.msborLsbFirst       = USCI_A_UART_LSB_FIRST;
    uart_params.numberofStopBits    = USCI_A_UART_ONE_STOP_BIT;
    uart_params.uartMode            = USCI_A_UART_MODE;
    uart_params.overSampling        = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;    // Clock = 1 MHz, Baudrate = 115200 bps (See [1])

    // UART initialization
    if (USCI_A_UART_init(USCI_A0_BASE, &uart_params) == STATUS_FAIL)
    {
#if DEBUG_MODE == true
        debug_PrintMsg("\tFAIL!");
#endif // DEBUG_MODE
        
        return STATUS_FAIL;
    }
    else
    {
        // Enable UART module
        USCI_A_UART_enable(USCI_A0_BASE);
        
        // Enable Receive Interrupt
        USCI_A_UART_clearInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
        USCI_A_UART_enableInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);

#if DEBUG_MODE == true
        debug_PrintMsg("\tSUCCESS!");
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
            //uint8_t receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            
            // Do something...
            
            break;
        default:
            break;
    }
}

//! \} End of eps_uart implementation group
