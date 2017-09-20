/*
 * time.c
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
 * \file time.c
 * 
 * \brief Time control module implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/06/2017
 * 
 * \addtogroup time
 * \{
 */

#include <config/config.h>
#include <libs/driverlib/driverlib.h>

#if BEACON_MODE != FLIGHT_MODE
    #include <modules/status_led/status_led.h>
#endif // FLIGHT_MODE

#if BEACON_MODE == DEBUG_MODE
    #include <modules/debug/debug.h>
#endif // DEBUG_MODE

#include "time.h"

uint32_t *second;

void time_init(uint32_t *second_ptr)
{
#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("Time control initialization... ");
#endif // DEBUG_MODE

    second = second_ptr;
    
    *second = 0;
    
    time_timer_init();

#if BEACON_MODE == DEBUG_MODE
    debug_print_msg("SUCCESS!\n");
#endif // DEBUG_MODE
}

static void time_timer_init()
{
    // Start timer in continuous mode sourced by SMCLK
    Timer_A_initContinuousModeParam timer_cont_params = {0};
    timer_cont_params.clockSource               = TIMER_A_CLOCKSOURCE_SMCLK;
    timer_cont_params.clockSourceDivider        = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    timer_cont_params.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    timer_cont_params.timerClear                = TIMER_A_DO_CLEAR;
    timer_cont_params.startTimer                = false;
    
    Timer_A_initContinuousMode(TIME_TIMER_BASE_ADDRESS, &timer_cont_params);
    
    // Initiaze compare mode
    Timer_A_clearCaptureCompareInterrupt(TIME_TIMER_BASE_ADDRESS, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    
    Timer_A_initCompareModeParam timer_comp_params = {0};
    timer_comp_params.compareRegister           = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    timer_comp_params.compareInterruptEnable    = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    timer_comp_params.compareOutputMode         = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    timer_comp_params.compareValue              = (uint16_t)(UCS_getSMCLK()/TIME_TIMER_COMP_VAL_DIVIDER);
    
    Timer_A_initCompareMode(TIME_TIMER_BASE_ADDRESS, &timer_comp_params);
}

void time_timer_start()
{
    Timer_A_startCounter(TIME_TIMER_BASE_ADDRESS, TIMER_A_CONTINUOUS_MODE);
}

void time_reset()
{
    *second = 0;
}

/**
 * \fn time_control_isr
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
void time_timer_isr()
{
    uint16_t comp_val = Timer_A_getCaptureCompareCount(TIME_TIMER_BASE_ADDRESS, TIMER_A_CAPTURECOMPARE_REGISTER_0)
                        + (uint16_t)(UCS_getSMCLK()/TIME_TIMER_COMP_VAL_DIVIDER);
    
    (*second)++;
    
#if BEACON_MODE != FLIGHT_MODE
    // Heartbeat
    status_led_toggle();
#endif // BEACON_MODE
    
    // Add Offset to CCR0
    Timer_A_setCompareValue(TIME_TIMER_BASE_ADDRESS, TIMER_A_CAPTURECOMPARE_REGISTER_0, comp_val);
    
    // Wake up from low power mode
    _BIC_SR(LPM1_EXIT);
}

//! \} End of time group
