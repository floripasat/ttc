/*
 * cpu.c
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
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
 * \file cpu.c
 * 
 * \brief CPU initialization functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/07/2017
 * 
 * \addtogroup cpu
 * \{
 */

#include <config/config.h>
#include <drivers/driverlib/driverlib.h>

#include "cpu.h"

#define UCS_MCLK_FLLREF_RATIO   (BEACON_CPU_FREQ_HZ/32768)

void cpu_init()
{
    // Set VCore = 2 for 12-20MHz clock
    PMM_setVCore(PMM_CORE_LEVEL_2);
    
    // Set DCO FLL reference = REFO
    UCS_initClockSignal(UCS_FLLREF, UCS_REFOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    // Set ACLK = REFO
    UCS_initClockSignal(UCS_ACLK, UCS_REFOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    // Set SMCLK = DCO/4
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_4);

    // Set Ratio and Desired MCLK Frequency  and initialize DCO
    UCS_initFLLSettle(BEACON_CPU_FREQ_KHZ, UCS_MCLK_FLLREF_RATIO);
    
    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
}

/**
 * \brief CPU clock failure ISR.
 * 
 * \return None
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(UNMI_VECTOR)))
#endif
void NMI_ISR()
{
    static uint16_t status = 0;
    
    do
    {
        // If it still can't clear the oscillator fault flags after the timeout,
        // trap and wait here.
        status = UCS_clearAllOscFlagsWithTimeout(1000);
    }
    while(status != 0);
}

//! \} End of cpu group
