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
 * \version 0.5.12
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

uint8_t radio_mode = RADIO_MODE_STANDBY;

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

        radio_mode = RADIO_MODE_STANDBY;

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

#if BEACON_TX_ENABLED == 1
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
        radio_mode = RADIO_MODE_STANDBY;
    #elif BEACON_RADIO == UART_SIM
        uart_radio_sim_send_data(data, len);
    #endif // BEACON_RADIO
#else
    debug_print_event_from_module(DEBUG_WARNING, RADIO_HAL_MODULE_NAME, "TRANSMISSIONS DISABLED!\n\r");
#endif // BEACON_TX_ENABLED
}

void radio_read(uint8_t *data, uint8_t len)
{
    if (len > 128)
    {
        len = 128;
    }

    debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Reading ");
    debug_print_dec(len);
    debug_print_msg(" bytes from buffer...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    return;
#elif BEACON_RADIO == SI4063
    return;
#elif BEACON_RADIO == RF4463F30
    rf4463_rx_packet(data, len);

    rf4463_clear_interrupts();

    radio_enable_rx();  // Returning to RX mode
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
    rf4463_enter_standby_mode();
    radio_mode = RADIO_MODE_STANDBY;
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
    if (radio_mode != RADIO_MODE_RX)
    {
        debug_print_event_from_module(DEBUG_INFO, RADIO_HAL_MODULE_NAME, "Enabling RX...\n\r");

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
        return;
#elif BEACON_RADIO == SI4063
        return;
#elif BEACON_RADIO == RF4463F30
        rf4463_fifo_reset();
        rf4463_enter_rx_mode();
        rf4463_clear_interrupts();
#elif BEACON_RADIO == UART_SIM
        return;
#endif // BEACON_RADIO

        radio_mode = RADIO_MODE_RX;
    }
}

void radio_disable_rx()
{
    radio_sleep();
}

bool radio_available()
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    return false;
#elif BEACON_RADIO == SI4063
    return false;
#elif BEACON_RADIO == RF4463F30
    return rf4463_wait_nIRQ();
#elif BEACON_RADIO == UART_SIM
    return false;
#endif // BEACON_RADIO
}

//! \} End of radio_hal group
