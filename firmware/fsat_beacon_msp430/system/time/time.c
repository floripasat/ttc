/*
 * time.c
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * \brief Time control module implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.6
 * 
 * \date 10/06/2017
 * 
 * \addtogroup time
 * \{
 */

#include <config/config.h>
#include <drivers/driverlib/driverlib.h>
#include <system/debug/debug.h>

#include "time.h"

Time time;

Time time_backup;

void time_init()
{
    debug_print_msg("Time control initialization... ");

    time_reset();
    
    time_timer_init();

    debug_print_msg("SUCCESS!\n\r");
}

static void time_timer_init()
{
    // Start timer in continuous mode sourced by SMCLK
    Timer_A_initContinuousModeParam timer_cont_params = {0};
    timer_cont_params.clockSource               = TIME_TIMER_CLOCK_SOURCE;
    timer_cont_params.clockSourceDivider        = TIME_TIMER_CLOCK_SOURCE_DIVIDER;
    timer_cont_params.timerInterruptEnable_TAIE = TIME_TIMER_INTERRUPT_ENABLE_TAIE;
    timer_cont_params.timerClear                = TIME_TIMER_CLEAR;
    timer_cont_params.startTimer                = false;
    
    Timer_A_initContinuousMode(TIME_TIMER_BASE_ADDRESS, &timer_cont_params);
    
    // Initiaze compare mode
    Timer_A_clearCaptureCompareInterrupt(TIME_TIMER_BASE_ADDRESS, TIME_TIMER_COMPARE_MODE);
    
    Timer_A_initCompareModeParam timer_comp_params = {0};
    timer_comp_params.compareRegister           = TIME_TIMER_COMPARE_REGISTER;
    timer_comp_params.compareInterruptEnable    = TIME_TIMER_COMPARE_INTERRUPT_ENABLE;
    timer_comp_params.compareOutputMode         = TIME_TIMER_COMPARE_OUTPUT_MODE;
    timer_comp_params.compareValue              = (uint16_t)(UCS_getSMCLK()/TIME_TIMER_COMPARE_DIVIDER_VALUE);
    
    Timer_A_initCompareMode(TIME_TIMER_BASE_ADDRESS, &timer_comp_params);
}

static uint8_t time_crc8(uint32_t time_counter)
{
    uint8_t time_in_bytes[4];
    time_in_bytes[0] = (uint8_t)((time_counter & 0xFF000000) >> 24);
    time_in_bytes[1] = (uint8_t)((time_counter & 0x00FF0000) >> 16);
    time_in_bytes[2] = (uint8_t)((time_counter & 0x0000FF00) >> 8);
    time_in_bytes[3] = (uint8_t)(time_counter & 0x000000FF);
    
    uint8_t crc = TIME_CRC8_INITIAL_VALUE;
    uint8_t i = 0;
    for(i=0; i<(32/8); i++)
    {
        crc ^= time_in_bytes[i];
        uint8_t j = 0;
        for (j=0; j<8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80)? TIME_CRC8_POLYNOMIAL: 0);
        }
        crc &= 0xFF;
    }
    
    return crc;
}

void time_timer_start()
{
    Timer_A_startCounter(TIME_TIMER_BASE_ADDRESS, TIME_TIMER_MODE);
}

void time_reset()
{
    time.second_counter = 0;
    time.crc8 = 0;
    
    time_backup.second_counter = time.second_counter;
    time_backup.crc8 = time.crc8;
}

uint32_t time_get_seconds()
{
    if (time_crc8(time.second_counter) != time.crc8)
    {
        if (time_crc8(time_backup.second_counter) == time_backup.crc8)
        {
            time.second_counter = time_backup.second_counter;
            time.crc8 = time_backup.crc8;
        }
        else
        {
            time_reset();
        }
    }
    
    return time.second_counter;
}

/**
 * \brief Time timer interrupt service routine.
 * 
 * The one second timer increments the time counters (seconds counter) and
 * wake up the CPU to run system cycle.
 * 
 * \return none.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIME_TIMER_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIME_TIMER_VECTOR)))
#endif
void time_timer_isr()
{
    uint16_t comp_val = Timer_A_getCaptureCompareCount(TIME_TIMER_BASE_ADDRESS, TIME_TIMER_COMPARE_REGISTER)
                        + (uint16_t)(UCS_getSMCLK()/TIME_TIMER_COMPARE_DIVIDER_VALUE);
    
    if (time_crc8(time.second_counter) == time.crc8)
    {
        time.second_counter++;
        time.crc8 = time_crc8(time.second_counter);
        
        time_backup.second_counter = time.second_counter;
        time_backup.crc8 = time.crc8;
    }
    else if (time_crc8(time_backup.second_counter) == time_backup.crc8)
    {
        time_backup.second_counter++;
        time_backup.crc8 = time_crc8(time_backup.second_counter);
        
        time.second_counter = time_backup.second_counter;
        time.crc8 = time_backup.crc8;
    }
    else
    {
        time_reset();
    }
    
    // Add Offset to CCR0
    Timer_A_setCompareValue(TIME_TIMER_BASE_ADDRESS, TIME_TIMER_COMPARE_REGISTER, comp_val);
    
    // Wake up from low power mode
    _BIC_SR(LOW_POWER_MODE_OFF);
}

//! \} End of time group
