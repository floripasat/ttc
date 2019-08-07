/*
 * eps_hal.c
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
 * \brief EPS HAL implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.13
 * 
 * \date 23/09/2016
 * 
 * \addtogroup eps_hal
 * \{
 */

#include <system/debug/debug.h>

#include "eps_hal.h"
#include "eps_hal_config.h"

Queue eps_queue;

bool eps_is_enabled = false;

bool eps_init()
{
    debug_print_event_from_module(DEBUG_INFO, EPS_HAL_MODULE_NAME, "Initializing communication bus...\n\r");

    // UART initialization
    if (eps_hal_uart_init())
    {
        queue_init(&eps_queue);

        return true;
    }
    else
    {
        return false;
    }
}

void eps_enable()
{
    if (!eps_is_enabled)
    {
        debug_print_event_from_module(DEBUG_INFO, EPS_HAL_MODULE_NAME, "Enabling reception...\n\r");

        USCI_A_UART_clearInterrupt(EPS_UART_BASE_ADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);
        USCI_A_UART_enableInterrupt(EPS_UART_BASE_ADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);

        eps_is_enabled = true;
    }
}

void eps_disable()
{
    if (eps_is_enabled)
    {
        debug_print_event_from_module(DEBUG_WARNING, EPS_HAL_MODULE_NAME, "Disabling reception...\n\r");

        USCI_A_UART_disableInterrupt(EPS_UART_BASE_ADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);

        eps_is_enabled = false;
    }
}

uint8_t eps_available()
{
    return queue_size(&eps_queue);
}

uint8_t eps_pop()
{
    return queue_pop_front(&eps_queue);
}

void eps_read(uint8_t *data, uint8_t bytes)
{
    uint8_t i = 0;
    for(i=0; i<bytes; i++)
    {
        data[i] = eps_pop();
    }
}

void eps_clear()
{
    while(eps_available())
    {
        eps_pop();
    }
}

static bool eps_hal_uart_init()
{
    debug_print_event_from_module(DEBUG_INFO, EPS_HAL_MODULE_NAME, "Initializing UART bus...\n\r");

    // UART pins init.
    GPIO_setAsPeripheralModuleFunctionInputPin(EPS_UART_RX_PORT, EPS_UART_RX_PIN);

    USCI_A_UART_initParam uart_params = {0};
    uart_params.selectClockSource   = EPS_HAL_CLOCK_SOURCE;
    uart_params.clockPrescalar      = EPS_HAL_UART_CLOCK_PRESCALAR;
    uart_params.firstModReg         = EPS_HAL_UART_FIRST_MOD_REG;
    uart_params.secondModReg        = EPS_HAL_UART_SECOND_MOD_REG;
    uart_params.parity              = EPS_HAL_UART_PARITY;
    uart_params.msborLsbFirst       = EPS_HAL_UART_ENDIENESS;
    uart_params.numberofStopBits    = EPS_HAL_UART_STOP_BITS;
    uart_params.uartMode            = EPS_HAL_UART_MODE;
    uart_params.overSampling        = EPS_HAL_UART_OVERSAMPLING;

    // UART initialization
    if (USCI_A_UART_init(EPS_UART_BASE_ADDRESS, &uart_params) == STATUS_SUCCESS)
    {
        // Enable UART module
        USCI_A_UART_enable(EPS_UART_BASE_ADDRESS);

        // Enable reception
        eps_enable();

        return true;
    }
    else
    {
        debug_print_event_from_module(DEBUG_ERROR, EPS_HAL_MODULE_NAME, "Error initializing the UART bus!\n\r");

        return false;
    }
}

static void eps_push(uint8_t byte)
{
    queue_push_back(&eps_queue, byte);
}

/**
 * \brief This is the USCI_A0 interrupt vector service routine.
 *
 * UART RX interruption routine.
 *
 * \image html eps_isr.png "EPS ISR flowchart" width=400px
 *
 * \return None.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=EPS_HAL_UART_ISR_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(EPS_HAL_UART_ISR_VECTOR)))
#endif
void USCI_A0_ISR()
{
    switch(__even_in_range(UCA0IV, 4))
    {
        // Vector 2 - RXIFG
        case 2:
            eps_push(USCI_A_UART_receiveData(EPS_UART_BASE_ADDRESS));
            break;
        default:
            break;
    }
}

//! \} End of eps_hal group
