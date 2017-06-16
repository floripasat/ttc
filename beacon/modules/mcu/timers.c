/*
 * timers.c
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
 * \file timers.c
 * 
 * \brief Initialization and control of the timers (implementation).
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/03/2017
 * 
 * \addtogroup timers
 * \{
 */

#include <config/config.h>
#include <libs/driverlib/driverlib.h>
#include <modules/modules.h>

#include "timers.h"

void timer_init()
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Main timer initialization... ");
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

#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("SUCCESS!\n");
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
    
    time.second++;
    if (time.second == 60)      // 1 minute = 60 seconds
    {
        time.minute++;
        time.second = 0;
    }
    if (time.minute == 60)      // 1 hour = 60 minutes
    {
        time.hour++;
        time.minute = 0;
    }
    if (time.hour == 24)        // 1 day = 24 hours
    {
        time.day++;
        time.hour = 0;
    }
    
#if BEACON_MODE != FLIGHT_MODE
    // Heartbeat
    status_led_toggle();
#endif // BEACON_MODE
    
    // Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, comp_val);
    
    // Wake up from low power mode
    _BIC_SR(LPM1_EXIT);
}

//! \} End of timers group
