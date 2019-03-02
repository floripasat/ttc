/*
 * radio_hal.c
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
 * \brief Radio HAL implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.12
 * 
 * \date 09/06/2017
 * 
 * \addtogroup radio_hal
 * \{
 */

#include <config/config.h>
#include <system/debug/debug.h>

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    #include <drivers/radio/cc11x5/cc11xx.h>
#elif BEACON_RADIO == SI4063
    #include <drivers/radio/si446x/si446x.h>
#elif BEACON_RADIO == RF4463F30
    #include <drivers/radio/rf4463/rf4463.h>
#elif BEACON_RADIO == UART_SIM
    #include <drivers/radio/uart_radio_sim/uart_radio_sim.h>
#endif // BEACON_RADIO

#if BEACON_RF_SWITCH != HW_NONE
    #include <hal/rf_switch/rf_switch.h>
#endif // BEACON_RF_SWITCH

#if BEACON_PA != HW_NONE
    #include <hal/pa/pa.h>
#endif // BEACON_PA

#include "radio_hal.h"
#include "radio_hal_config.h"

Queue radio_rx_queue;

bool radio_init()
{
    debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Initializing device...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    uint8_t init_status = cc11xx_init();
    if (init_status == STATUS_SUCCESS)
    {
        cc11xx_ManualCalibration();

        return true;
    }
    else
    {
        return false;
    }
#elif BEACON_RADIO == SI4063
    return si406x_init();
#elif BEACON_RADIO == RF4463F30
    uint8_t init_status = rf4463_init();
    if (init_status == STATUS_SUCCESS)
    {
        rf4463_enter_standby_mode();
        return true;
    }
    else
    {
        return false;
    }
#elif BEACON_RADIO == UART_SIM
    return uart_radio_sim_init(UART_RADIO_ONLY_SET_TX_PIN_AS_PERIPHERAL);
#endif // BEACON_RADIO
}

void radio_reset()
{
    debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Reseting...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125

#elif BEACON_RADIO == SI4063

#elif BEACON_RADIO == RF4463F30
    rf4463_init();
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

void radio_write(uint8_t *data, uint16_t len)
{
    debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Writing ");
    debug_print_dec(len);
    debug_print_msg(" bytes to the buffer...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    #if BEACON_RF_SWITCH != HW_NONE
        rf_switch_enable_beacon();
    #endif // BEACON_RF_SWITCH
        
    #if BEACON_PA != HW_NONE
        pa_enable();
    #endif // BEACON_PA
    
    //cc11xx_write();
    
    #if BEACON_PA != HW_NONE
        pa_disable();
    #endif // BEACON_PA
        
    #if BEACON_RF_SWITCH != HW_NONE
        rf_switch_disable_beacon();
    #endif // BEACON_RF_SWITCH
    
#elif BEACON_RADIO == SI4063
    
#elif BEACON_RADIO == RF4463F30
    rf4463_tx_long_packet(data, len);
    rf4463_enter_standby_mode();
#elif BEACON_RADIO == UART_SIM
    uart_radio_sim_send_data(data, len);
#endif // BEACON_RADIO
}

void radio_read(uint8_t len)
{
    debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Reading ");
    debug_print_dec(len);
    debug_print_msg(" bytes from buffer...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125

#elif BEACON_RADIO == SI4063

#elif BEACON_RADIO == RF4463F30
    uint8_t data[128];
    rf4463_rx_packet(data, len);

    uint8_t i = 0;
    for(i=0; i<len; i++)
    {
        radio_push(data[i]);
    }

#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

void radio_sleep()
{
    debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Entering sleep mode...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125

#elif BEACON_RADIO == SI4063

#elif BEACON_RADIO == RF4463F30
    GPIO_disableInterrupt(RADIO_GPIO_nIRQ_PORT, RADIO_GPIO_nIRQ_PIN);
    rf4463_enter_standby_mode();
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

void radio_wake_up()
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125

#elif BEACON_RADIO == SI4063

#elif BEACON_RADIO == RF4463F30
    return;
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

void radio_enable_rx()
{
    debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Enabling RX...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    return;
#elif BEACON_RADIO == SI4063
    return;
#elif BEACON_RADIO == RF4463F30
    radio_init_rx_isr();
    rf4463_enter_rx_mode();
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

void radio_disable_rx()
{
    radio_sleep();
}

uint8_t radio_available()
{
    return queue_size(&radio_rx_queue);
}

uint8_t radio_pop()
{
    return queue_pop_front(&radio_rx_queue);
}

static void radio_init_rx_isr()
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    return;
#elif BEACON_RADIO == SI4063
    return;
#elif BEACON_RADIO == RF4463F30
    // Enables interrupt
    GPIO_enableInterrupt(RADIO_GPIO_nIRQ_PORT, RADIO_GPIO_nIRQ_PIN);

    // Sets Hi/Lo edge
    GPIO_selectInterruptEdge(RADIO_GPIO_nIRQ_PORT, RADIO_GPIO_nIRQ_PIN, GPIO_HIGH_TO_LOW_TRANSITION);

    // Clears IFG
    GPIO_clearInterrupt(RADIO_GPIO_nIRQ_PORT, RADIO_GPIO_nIRQ_PIN);
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

static void radio_push(uint8_t byte)
{
    queue_push_back(&radio_rx_queue, byte);
}

#if BEACON_RADIO == RF4463F30
/**
 * \brief Radio RX interrupt vector service routine.
 * 
 * \return None
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RADIO_HAL_RX_ISR_PORT_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RADIO_HAL_RX_ISR_PORT_VECTOR)))
#endif
void radio_rx_isr()
{
    radio_read(50);

    // P3.1 IFG cleared
    GPIO_clearInterrupt(RADIO_GPIO_nIRQ_PORT, RADIO_GPIO_nIRQ_PIN);
}
#endif // BEACON_RADIO

//! \} End of radio_hal group
