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

uint8_t obdh_gpio_state = OBDH_GPIO_STATE_WAITING_BIT0;

void obdh_GPIO_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Initializing OBDH communication... ");
#endif // DEBUG_MODE

    // Enable GPIO pinn as input pins
    GPIO_setAsInputPin(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3);
    
    // OBDH_GPIO_PIN0 interrupt enabled
    GPIO_enableInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_PIN0);
    
    // OBDH_GPIO_PIN0 Hi/Lo edge
    GPIO_selectInterruptEdge(OBDH_GPIO_PORT, OBDH_GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);

    // OBDH_GPIO_PIN0 IFG cleared
    GPIO_clearInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_PIN0);

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

//! \} End of gpio-obdh implementation group
