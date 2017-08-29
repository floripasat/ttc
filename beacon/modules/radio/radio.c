/*
 * radio.c
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
 * \file radio.c
 * 
 * \brief Radio module implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 09/06/2017
 * 
 * \addtogroup radio
 * \{
 */

#include <config/config.h>
#include <src/tasks.h>

#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    #include <drivers/radio/cc11x5/cc11xx.h>
#elif BEACON_RADIO == SI4063
    #include <drivers/radio/si446x/si446x.h>
#elif BEACON_RADIO == RF4463F30
    #include <drivers/radio/rf4463/rf4463.h>
#elif BEACON_RADIO == UART_SIM
    #include <drivers/radio/uart_radio_sim/uart_radio_sim.h>
#endif // BEACON_RADIO

#include "radio.h"

uint8_t radio_init()
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    uint8_t init_status = cc11xx_init();
    if (init_status == STATUS_SUCCESS)
    {
        cc11xx_ManualCalibration();
        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
#elif BEACON_RADIO == SI4063
    return si406x_init();
#elif BEACON_RADIO == RF4463F30
    uint8_t init_status = rf4463_init();
    if (init_status == STATUS_SUCCESS)
    {
        rf4463_enter_standby_mode();
        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
#elif BEACON_RADIO == UART_SIM
    return uart_radio_sim_init(UART_RADIO_ONLY_SET_TX_PIN_AS_PERIPHERAL);
#endif // BEACON_RADIO
}

void radio_reset()
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    
#elif BEACON_RADIO == SI4063
    
#elif BEACON_RADIO == RF4463F30
    rf4463_init();
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

void radio_write_data(uint8_t *data, uint16_t len)
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    
#elif BEACON_RADIO == SI4063
    
#elif BEACON_RADIO == RF4463F30
    rf4463_tx_long_packet(data, len);
    rf4463_enter_standby_mode();
#elif BEACON_RADIO == UART_SIM
    uart_radio_sim_send_data(data, len);
#endif // BEACON_RADIO
}

void radio_read_data(uint8_t *buffer, uint8_t *len)
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    
#elif BEACON_RADIO == SI4063
    
#elif BEACON_RADIO == RF4463F30
    *len = rf4463_rx_packet(buffer);
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

void radio_sleep()
{
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

void radio_init_rx_isr()
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

void radio_enable_rx()
{
#if BEACON_RADIO == CC1175 || BEACON_RADIO == CC1125
    return;
#elif BEACON_RADIO == SI4063
    return;
#elif BEACON_RADIO == RF4463F30
    rf4463_enter_rx_mode();
#elif BEACON_RADIO == UART_SIM
    return;
#endif // BEACON_RADIO
}

#if BEACON_RADIO == RF4463F30
/**
 * \fn Port_3
 * 
 * \brief This is the PORT3_VECTOR interrupt vector service routine.
 * 
 * \return None
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT3_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT3_VECTOR)))
#endif
void Port_3()
{
    uint8_t pkt[100];
    uint8_t pkt_len = rf4463_rx_packet(pkt);
    
    task_receive_packet(pkt, pkt_len);
    
    // P3.1 IFG cleared
    GPIO_clearInterrupt(RADIO_GPIO_nIRQ_PORT, RADIO_GPIO_nIRQ_PIN);
}
#endif // BEACON_RADIO

//! \} End of radio group
