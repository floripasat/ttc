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

void timer_Init()
{
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
}

//! \} End of timer group
