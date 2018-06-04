/*
 * tasks.h
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
 * \brief Tasks handler functions.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \defgroup tasks Tasks
 * \ingroup system
 * \{
 */

#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * \brief A pointer to a void function without input parameters.
 */
typedef void (*task_ptr)();

/**
 * \brief A pointer to a bool function without input parameters.
 */
typedef bool (*bool_task_ptr)();

/**
 * \brief Runs a task periodically.
 * 
 * \param task is a pointer to a task (function) without input parameters (void).
 * \param period_s is period of the task in seconds.
 * \param last_execution_s is a pointer to the last time, in seconds, of the task execution.
 * \param current_time is the current system time in seconds.
 * 
 * \return None.
 */
void task_periodic(task_ptr task, uint32_t period_s, uint32_t *last_execution_s, uint32_t current_time_s);

/**
 * \brief Runs a task periodically without preemption during the task execution.
 * 
 * Before the execution of the task, all the interruptions are disabled. After the task execution, all the
 * interruptions are enabled again.
 * 
 * \param task is a pointer to a task (function) without input parameters (void).
 * \param period_s is period of the task in seconds.
 * \param last_execution_s is a pointer to the last time, in seconds, of the task execution.
 * \param current_time is the current system time in seconds.
 * 
 * \return None.
 */
void task_periodic_no_preemption(task_ptr task, uint32_t period_s, uint32_t *last_execution_s, uint32_t current_time_s);

/**
 * \brief Executes an aperiodic task, in agreement of a condition.
 * 
 * For the condition of execution, an unary operator can be used.
 * 
 * \param task is a pointer to a task (function) without input parameters (void).
 * \param condition is the condition to executer the task (if true, executes, if false, not).
 * 
 * \return None.
 */
void task_aperiodic(task_ptr task, bool condition);

/**
 * \brief Executes an scheduled task, in agreement of a condition.
 * 
 * For the condition of execution, an unary operator can be used.
 * 
 * \param task is a pointer to a task (function) without input parameters (void).
 * \param time_to_run_s is the time scheduled to run the task (in seconds).
 * \param current_time is the current system time in seconds.
 * \param timeout_s is the timeout, in seconds, to run the task.
 * \param condition is the condition to executer the task (if true, executes, if false, not).
 * 
 * \return None.
 */
void task_scheduled(task_ptr task, uint32_t time_to_run_s, uint32_t current_time_s, uint16_t timeout_s, bool condition);

/**
 * \brief Executes an scheduled task without preemption, in agreement of a condition.
 * 
 * For the condition of execution, an unary operator can be used.
 * 
 * Before the execution of the task, all the interruptions are disabled. After the task execution, all the
 * interruptions are enabled again.
 * 
 * \param task is a pointer to a task (function) without input parameters (void).
 * \param time_to_run_s is the time scheduled to run the task (in seconds).
 * \param current_time is the current system time in seconds.
 * \param timeout_s is the timeout, in seconds, to run the task.
 * \param condition is the condition to executer the task (if true, executes, if false, not).
 * 
 * \return None.
 */
void task_scheduled_no_preemption(task_ptr task, uint32_t time_to_run_s, uint32_t current_time_s, uint16_t timeout_s, bool condition);

/**
 * \brief Executes a initialization routine with timeout.
 * 
 * \param task is a pointer to a task (function returning a bool type) without input parameters (void).
 * \param timeout_ms is the task timeout in milliseconds.
 * 
 * \return None.
 */
void task_init_with_timeout(bool_task_ptr task, uint32_t timeout_s);

#endif // TASKS_H_

//! \} End of tasks group
