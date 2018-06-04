/*
 * tasks.c
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \brief Tasks handler functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \addtogroup tasks
 * \{
 */

#include <msp430.h>
#include <config/config.h>

#include "tasks.h"

void task_periodic(task_ptr task, uint32_t period_s, uint32_t *last_execution_s, uint32_t current_time_s)
{
    if ((current_time_s - (*last_execution_s)) >= period_s)
    {
        task();                                 // Executes the task
        
        *last_execution_s = current_time_s;     // Saves the time of the task execution
    }
}

void task_periodic_no_preemption(task_ptr task, uint32_t period_s, uint32_t *last_execution_s, uint32_t current_time_s)
{
    if ((current_time_s - (*last_execution_s)) >= period_s)
    {
        __disable_interrupt();                  // Disables global interrupts
        
        task();                                 // Executes the task
        
        *last_execution_s = current_time_s;     // Saves the time of the task execution
        
        __enable_interrupt();                   // Enables global interrupts
    }
}

void task_aperiodic(task_ptr task, bool condition)
{
    if (condition == true)
    {
        task();
    }
}

void task_scheduled(task_ptr task, uint32_t time_to_run_s, uint32_t current_time_s, uint16_t timeout_s, bool condition)
{
    if (condition == true)
    {
        if (current_time_s >= time_to_run_s)
        {
            if ((time_to_run_s + timeout_s) <= current_time_s)
            {
                task();
            }
        }
    }
}

void task_scheduled_no_preemption(task_ptr task, uint32_t time_to_run_s, uint32_t current_time_s, uint16_t timeout_s, bool condition)
{
    if (condition == true)
    {
        if (current_time_s >= time_to_run_s)
        {
            if ((time_to_run_s + timeout_s) <= current_time_s)
            {
                __disable_interrupt();                  // Disables global interrupts
                
                task();
                
                __enable_interrupt();                   // Enables global interrupts
            }
        }
    }
}

void task_init_with_timeout(bool_task_ptr task, uint32_t timeout_ms)
{
    while((!task()) && (timeout_ms--))
    {
        __delay_cycles(BEACON_CPU_FREQ_HZ/1000);
    }
}

//! \} End of tasks group
