/*
 * timer.c
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
 * along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file timer.c
 * 
 * \brief Initialization and control of timer A.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/03/2017
 * 
 * \addtogroup timer
 * \{
 */

#include "../inc/timer.h"
#include "../inc/debug.h"
#include "../driverlib/driverlib.h"
#include "../inc/led.h"

// Initialization of the time counters
uint8_t timer_sec_counter = 0;
uint8_t timer_min_counter = 0;
uint8_t timer_hour_counter = 0;
uint16_t timer_day_counter = 0;

void timer_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Main timer initialization... ");
#endif // DEBUG_MODE

    // Start timer in continuous mode sourced by SMCLK
    Timer_A_initContinuousModeParam initContParam = {0};
    initContParam.clockSource               = TIMER_A_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider        = TIMER_A_CLOCKSOURCE_DIVIDER_20;
    initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParam.timerClear                = TIMER_A_DO_CLEAR;
    initContParam.startTimer                = false;
    
    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);
    
    // Initiaze compare mode
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    
    Timer_A_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister           = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable    = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam.compareOutputMode         = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initCompParam.compareValue              = (uint16_t)(UCS_getSMCLK()/TIMER_A_CLOCKSOURCE_DIVIDER_20);
    
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

/**
 * \fn TIMER1_A0_ISR
 *
 * \brief Timer A0 interrupt service routine.
 * 
 * The one second timer increments the time counters (second and minute) and
 * wake up the CPU to verify the elapsed time.
 * 
 * \return none
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMER1_A0_ISR()
{
    uint16_t comp_val = Timer_A_getCaptureCompareCount(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0)
                        + (uint16_t)(UCS_getSMCLK()/TIMER_A_CLOCKSOURCE_DIVIDER_20);
    
    timer_sec_counter++;
    if (timer_sec_counter == 60)        // 1 minute = 60 seconds
    {
        timer_min_counter++;
        timer_sec_counter = 0;
    }
    if (timer_min_counter == 60)        // 1 hour = 60 minutes
    {
        timer_hour_counter++;
        timer_min_counter = 0;
    }
    if (timer_hour_counter == 24)       // 1 day = 24 hours
    {
        timer_day_counter++;
        timer_hour_counter = 0;
    }
    
    // Heartbeat
    led_Blink();
    
    // Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, comp_val);
    
    // Wake up from low power mode
    _BIC_SR(LPM1_EXIT);
}

//! \} End of timer group
