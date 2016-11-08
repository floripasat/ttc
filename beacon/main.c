/*
 * main.c
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
 * \file main.c
 * 
 * \brief Main file
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup beacon Beacon
 * \{
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "driverlib/driverlib.h"

#include "inc/debug.h"
#include "inc/watchdog.h"
#include "inc/led.h"
#include "inc/cc11xx.h"
#include "inc/rf-switch.h"
#include "inc/rf6886.h"
#include "inc/uart-eps.h"
#include "inc/antenna.h"
#include "inc/delay.h"

#define ADDRESS     0x17            /**< Address byte. */
#define TX_MESSAGE  "FloripaSat"    /**< Message to transmit. */

#define START_OF_DATA       0x7E
#define END_OF_DATA         0x98

// UART-EPS interruption variables
uint8_t received_byte = 0;
uint8_t received_data[10];
uint8_t data_counter = 0;
uint8_t data_ready = 0;
uint8_t receiving = 0;

/**
 * \fn main
 * 
 * \brief The main function.
 * 
 * After the initializations of the periphericals,
 * the program stays running in infinite loop.
 * 
 * \return None
 */
void main()
{
    led_Init();
    led_Enable();
    
#if DEBUG_MODE == true
    WDT_A_hold(WDT_A_BASE);         // Disable watchdog for debug
    
    // UART for debug
    while(debug_Init() != STATUS_SUCCESS)
    {
        // Blinking system LED if something is wrong
        led_Blink(1000);
    }
#else
    watchdog_Init();
#endif // DEBUG_MODE
    
    // UART for EPS data
    while(eps_UART_Init() != STATUS_SUCCESS)
    {
        // Blinking system LED if something is wrong
        led_Blink(1000);
    }
    
    // Antenna deployment
    antenna_Init();
    
    // Radio initialization
    cc11xx_Init();

    // Calibrate radio (See "CC112X, CC1175 Silicon Errata")
    cc11xx_ManualCalibration();
 
    // Beacon PA initialization
    while(rf6886_Init() != STATUS_SUCCESS)
    {
        // Blinking system LED if something is wrong
        led_Blink(1000);
    }
    rf_switch_Init();

    rf6886_Enable();
    rf6886_SetVreg(3.1);            // DAC output = 3,1V
    rf_switch_Enable();

    // Data to send
    uint8_t address = ADDRESS;
    uint8_t tx_buffer[] = TX_MESSAGE;

    // Infinite loop
    while(1)
    {
#if DEBUG_MODE == false
        WDT_A_resetTimer(WDT_A_BASE);
#endif // DEBUG_MODE

        // Flush the TX FIFO
        cc11xx_CmdStrobe(CC11XX_SFTX);

        // Write packet to TX FIFO
        cc11xx_WriteTXFIFO(&address, 1);                        // The first byte written to the TXFIFO should be the address
        cc11xx_WriteTXFIFO(tx_buffer, sizeof(tx_buffer)-1);     // -1 = '\0'

        // Enable TX (Command strobe)
        cc11xx_CmdStrobe(CC11XX_STX);

        // Heartbeat
        led_Blink(100);
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
            received_byte = USCI_A_UART_receiveData(USCI_A0_BASE);

            switch(received_byte)
            {
                case START_OF_DATA:
                    receiving = 1;
                    data_counter = 0;
                    received_data[data_counter] = received_byte;
                    data_counter++;
                    data_ready = 0;
                    break;
                case END_OF_DATA:
                    if (receiving)
                    {
                        received_data[data_counter] = received_byte;
                        data_counter++;
                        receiving  = 0;
                        data_ready = 1;
                    }
                    break;
                default:
                    if (receiving)
                    {
                        received_data[data_counter] = received_byte;
                        data_counter++;
                    }
                    break;
            }

            break;
        default:
            break;
    }
}

//! \} End of beacon group
