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

#include "../inc/gpio-obdh.h"
#include "../inc/debug.h"
#include "../driverlib/driverlib.h"
#include "../inc/sleep_mode.h"

uint8_t obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT0;

void obdh_GPIO_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Initializing OBDH communication peripherals... ");
#endif // DEBUG_MODE

    // Enable GPIO pinn as input pins
    GPIO_setAsInputPinWithPullUpResistor(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3);
    
    // OBDH_GPIO_PIN0 interrupt enabled
    GPIO_enableInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3);
    
    // OBDH_GPIO_PIN0 Hi/Lo edge
    GPIO_selectInterruptEdge(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3, GPIO_LOW_TO_HIGH_TRANSITION);

    // OBDH_GPIO_PIN0 IFG cleared
    GPIO_clearInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3);

    obdh_GPIO_Timer_Init();

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

void obdh_GPIO_Timer_Init()
{
    // Start timer in continuous mode sourced by SMCLK
    Timer_B_initContinuousModeParam initContParam = {0};
    initContParam.clockSource               = TIMER_B_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider        = TIMER_B_CLOCKSOURCE_DIVIDER_40;
    initContParam.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    initContParam.timerClear                = TIMER_B_DO_CLEAR;
    initContParam.startTimer                = false;
    
    Timer_B_initContinuousMode(TIMER_B0_BASE, &initContParam);
    
    // Initiaze compare mode
    Timer_B_clearCaptureCompareInterrupt(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_0);
    
    Timer_B_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister           = TIMER_B_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable    = TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam.compareOutputMode         = TIMER_B_OUTPUTMODE_OUTBITVALUE;
    initCompParam.compareValue              = OBDH_GPIO_MAX_TRANSMISSION_TIME_SEC*(uint16_t)(UCS_getSMCLK()/TIMER_B_CLOCKSOURCE_DIVIDER_40);
    
    Timer_B_initCompareMode(TIMER_B0_BASE, &initCompParam);
}

/**
 * \fn TIMER1_B0_ISR
 *
 * \brief Timer B0 interrupt service routine.
 * 
 * This timer is triggered when the data transmission between OBDH and BEACON starts.
 * It establish a time limit of transmission between OBDH and BEACON.
 * 
 * \return none
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERB0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMERB0_VECTOR)))
#endif
void TIMERB0_ISR()
{
    uint16_t comp_val = Timer_B_getCaptureCompareCount(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_0)
	                    + OBDH_GPIO_MAX_TRANSMISSION_TIME_SEC*(uint16_t)(UCS_getSMCLK()/TIMER_B_CLOCKSOURCE_DIVIDER_40);
    
    // Go back to the initial state
    obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT0;
    
    // Add Offset to CCR0
    Timer_B_setCompareValue(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_0, comp_val);
    
    // Stop timer to wait for the next attempt
    Timer_B_stop(TIMER_B0_BASE);
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
    
    switch(obdh_gpio_state)
    {
        case OBDH_GPIO_STATE_WAITING_BIT0:
            if ((GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN0) == GPIO_INPUT_PIN_HIGH) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN1) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN2) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN3) == GPIO_INPUT_PIN_LOW))
            {
                // Jump to the next state
                obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT1;
                
                // Init. timer (2s to receive all the data)
                Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_CONTINUOUS_MODE);
                
                break;
            }
            else
            {
                obdh_gpio_state = OBDH_GPIO_STATE_ERROR;
            }
        case OBDH_GPIO_STATE_WAITING_BIT1:
            if ((GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN0) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN1) == GPIO_INPUT_PIN_HIGH) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN2) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN3) == GPIO_INPUT_PIN_LOW))
            {
                // Jump to the next state
                obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT2;
                
                break;
            }
            else
            {
                obdh_gpio_state = OBDH_GPIO_STATE_ERROR;
            }
        case OBDH_GPIO_STATE_WAITING_BIT2:
            if ((GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN0) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN1) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN2) == GPIO_INPUT_PIN_HIGH) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN3) == GPIO_INPUT_PIN_LOW))
            {
                // Jump to the next state
                obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT3;
                
                break;
            }
            else
            {
                obdh_gpio_state = OBDH_GPIO_STATE_ERROR;
            }
        case OBDH_GPIO_STATE_WAITING_BIT3:
            if ((GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN0) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN1) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN2) == GPIO_INPUT_PIN_LOW) &&
                (GPIO_getInputPinValue(OBDH_GPIO_PORT, OBDH_GPIO_PIN3) == GPIO_INPUT_PIN_HIGH))
            {
                // Go back to the initial state
                obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT0;
                
                sleep_mode_TurnOn();
                
                break;
            }
            else
            {
                obdh_gpio_state = OBDH_GPIO_STATE_ERROR;
            }
        default:
            // Go back to the initial state
            obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT0;
            
            // Stop timer to wait for the next attempt
            Timer_B_stop(TIMER_B0_BASE);
            
            break;
    };
    
    GPIO_clearInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3);
}

//! \} End of gpio-obdh implementation group
