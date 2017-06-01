/*
 * gpio-obdh.c
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \file gpio-obdh.c
 * 
 * \brief Implementation of the functions to receive the 24 hours shutdown command from ODBH.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/03/2017
 * 
 * \addtogroup gpio-obdh
 * \{
 */

#include <stdbool.h>

#include "../inc/gpio-obdh.h"
#include "../inc/debug.h"
#include "../inc/sleep_mode.h"

uint8_t obdh_gpio_obdh_state = OBDH_GPIO_OBDH_TX_OFF;

void obdh_GPIO_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Initializing OBDH communication peripherals... ");
#endif // DEBUG_MODE

    // Enable OBDH_GPIO_INTERRUPT_PIN, OBDH_GPIO_SHUTDOWN_PIN and OBDH_GPIO_TX_REQUEST_PIN as input pins
    GPIO_setAsInputPinWithPullDownResistor(OBDH_GPIO_PORT, OBDH_GPIO_INTERRUPT_PIN + OBDH_GPIO_SHUTDOWN_PIN + OBDH_GPIO_TX_REQUEST_PIN);
    
    // Enable OBDH_GPIO_TX_BUSY_PIN as output pin
    GPIO_setAsOutputPin(OBDH_GPIO_PORT, OBDH_GPIO_TX_BUSY_PIN);
    GPIO_setOutputLowOnPin(OBDH_GPIO_PORT, OBDH_GPIO_TX_BUSY_PIN);
    
    // Enable interrupt on OBDH_GPIO_INTERRUPT_PIN
    GPIO_enableInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_INTERRUPT_PIN);
    
    // Set low to high transitions on OBDH_GPIO_INTERRUPT_PIN
    GPIO_selectInterruptEdge(OBDH_GPIO_PORT, OBDH_GPIO_INTERRUPT_PIN, GPIO_LOW_TO_HIGH_TRANSITION);

    // OBDH_GPIO_INTERRUPT_PIN IFG cleared
    GPIO_clearInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_INTERRUPT_PIN);

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

void obdh_GPIO_SendToOBDH(uint8_t msg)
{
    switch(msg)
    {
        case OBDH_GPIO_BEACON_TX_ON:
            GPIO_setOutputHighOnPin(OBDH_GPIO_PORT, OBDH_GPIO_TX_BUSY_PIN);
            break;
        case OBDH_GPIO_BEACON_TX_OFF:
            GPIO_setOutputLowOnPin(OBDH_GPIO_PORT, OBDH_GPIO_TX_BUSY_PIN);
            break;
        default:
            GPIO_setOutputLowOnPin(OBDH_GPIO_PORT, OBDH_GPIO_TX_BUSY_PIN);
    };
}

/**
 * \fn Port_4
 *
 * \brief This is the PORT4_VECTOR interrupt vector service routine.
 *
 * OBDH GPIO communication interruption routine.
 *
 * \return none
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT4_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT4_VECTOR)))
#endif
void Port_4()
{
    // Delay for the settling time of the signal
    _delay_cycles(100);
    
    if (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_INTERRUPT_PIN) == GPIO_INPUT_PIN_HIGH)
    {
#if DEBUG_MODE == true
        debug_PrintMsg("Interrupt received from OBDH!\n");
#endif // DEBUG_MODE
        if (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_SHUTDOWN_PIN) == GPIO_INPUT_PIN_HIGH)
        {
#if DEBUG_MODE == true
            debug_PrintMsg("Shutdown command received from OBDH!\n");
#endif // DEBUG_MODE
            if (sleep_mode_getStatus() == BEACON_SLEEP_MODE_OFF)
            {
                sleep_mode_TurnOn();
            }
        }
        
        if (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_TX_REQUEST_PIN) == GPIO_INPUT_PIN_HIGH)
        {
#if DEBUG_MODE == true
            debug_PrintMsg("TX request received from OBDH!\n");
#endif // DEBUG_MODE
            obdh_gpio_obdh_state = OBDH_GPIO_OBDH_TX_ON;
        }
        else
        {
            obdh_gpio_obdh_state = OBDH_GPIO_OBDH_TX_OFF;
        }
    }
    
    GPIO_clearInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_INTERRUPT_PIN);
}

//! \} End of gpio-obdh implementation group
