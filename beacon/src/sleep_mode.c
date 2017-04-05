/*
 * sleep_mode.c
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
 * \file sleep_mode.c
 * 
 * \brief Sleep mode control functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 24/03/2017
 * 
 * \addtogroup sleep_mode
 * \{
 */

#include "../inc/sleep_mode.h"
#include "../driverlib/driverlib.h"
#include "../inc/timer.h"
#include "../inc/cc11xx.h"
#include "../inc/gpio-obdh.h"
#include "../inc/debug.h"

uint8_t beacon_sleep_mode = BEACON_SLEEP_MODE_OFF;
uint8_t beacon_sleep_mode_initial_hour = 0;

void sleep_mode_TurnOn()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Entering sleep mode... ");
#endif // DEBUG_MODE

    GPIO_disableInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3);

    beacon_sleep_mode = BEACON_SLEEP_MODE_ON;
    
    beacon_sleep_mode_initial_hour = timer_hour_counter;
    
    // Putting radio in sleep mode
    cc11xx_CmdStrobe(CC11XX_SPWD);

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

void sleep_mode_TurnOff()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Leaving sleep mode... ");
#endif // DEBUG_MODE

    GPIO_enableInterrupt(OBDH_GPIO_PORT, OBDH_GPIO_PIN0 + OBDH_GPIO_PIN1 + OBDH_GPIO_PIN2 + OBDH_GPIO_PIN3);

    beacon_sleep_mode = BEACON_SLEEP_MODE_OFF;
    
    beacon_sleep_mode_initial_hour = 0;
    
    // Radio returns to idle state again
    cc11xx_WakeUp();

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

uint8_t sleep_mode_getStatus()
{
    return beacon_sleep_mode;
}

//! \} End of sleep_mode implementation group
