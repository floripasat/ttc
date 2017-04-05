/*
 * main.c
 * 
 * Copyright (C) 2016, Universidade Federal de Santa Catarina
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
 * \file main.c
 * 
 * \brief Main file
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup beacon Beacon
 * \{
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "driverlib/driverlib.h"

#include "inc/debug.h"
#include "inc/watchdog.h"
#include "inc/led.h"
#include "inc/cc11xx.h"
#include "inc/rf-switch.h"
#include "inc/rf6886.h"
#include "inc/uart-eps.h"
#include "inc/gpio-obdh.h"
#include "inc/antenna.h"
#include "inc/ax25.h"
#include "inc/pkt_payload.h"
#include "inc/timer.h"
#include "inc/sleep_mode.h"

#define BEACON_ANTENNA_DEPLOY_SLEEP_MIN     45
#define BEACON_PKT_PERIOD_SEC               30

/**
 * \fn main
 * 
 * \brief The main function.
 * 
 * After the initializations of the periphericals,
 * the program stays running in infinite loop.
 * 
 * \return None
 */
void main()
{
#if DEBUG_MODE == true
    WDT_A_hold(WDT_A_BASE);         // Disable watchdog for debug
    
    // UART for debug
    while(debug_Init() != STATUS_SUCCESS)
    {
        
    }
#else
    watchdog_Init();
#endif // DEBUG_MODE
    
    // 1 second timer initialization
    timer_Init();
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
    
    // Antenna initialization
    while(antenna_Init() != STATUS_SUCCESS)
    {
        
    }
    
    // Verify if the antenna is released
    if (!antenna_IsReleased())
    {
        uint8_t antenna_deployment_min_counter = timer_min_counter;        
        while((uint8_t)((timer_min_counter - antenna_deployment_min_counter) % 60) < BEACON_ANTENNA_DEPLOY_SLEEP_MIN)
        {
#if DEBUG_MODE == false
            WDT_A_resetTimer(WDT_A_BASE);
#endif // DEBUG_MODE

            // Enter LPM1 with interrupts enabled
            _BIS_SR(LPM1_bits + GIE);
        }
        
        // Antenna deployment
        while(antenna_Release() != STATUS_SUCCESS)
        {
            
        }
    }
    
    // UART for EPS data
    while(eps_UART_Init() != STATUS_SUCCESS)
    {
        
    }
    
    // OBDH GPIO communication initialization
    obdh_GPIO_Init();
    
    // Radio initialization
    while(cc11xx_Init() != STATUS_SUCCESS)
    {
        
    }
 
    // Calibrate radio (See "CC112X, CC1175 Silicon Errata")
    cc11xx_ManualCalibration();
    
    // Beacon PA initialization
    while(rf6886_Init() != STATUS_SUCCESS)
    {
        
    }
    
    // RF switch initialization
    rf_switch_Init();

    rf6886_SetVreg(3.1);            // DAC output = 3,1V

    // Data to send
    AX25_Packet ax25_packet;
    uint8_t pkt_payload[pkt_payload_GetSize(eps_data) + 1];
    uint8_t str_packet[AX25_FLORIPASAT_HEADER_SIZE + sizeof(pkt_payload)];

    // Status LED initialization
    led_Init();

    // Infinite loop
    while(1)
    {        
        if (sleep_mode_getStatus() == BEACON_SLEEP_MODE_ON)
        {
            if ((uint8_t)((timer_hour_counter - beacon_sleep_mode_initial_hour) % 24) >= BEACON_SLEEP_MODE_PERIOD_HOUR)
            {
                sleep_mode_TurnOff();
            }
        }
        else
        {
            pkt_payload_gen(pkt_payload, eps_data);
            ax25_BeaconPacketGen(&ax25_packet, pkt_payload, sizeof(pkt_payload)-1);
            ax25_Packet2String(&ax25_packet, pkt_payload, sizeof(pkt_payload)-1);
            
            // Flush the TX FIFO
            cc11xx_CmdStrobe(CC11XX_SFTX);
            
            // Enable the switch and the PA
            rf6886_Enable();
            rf_switch_Enable();

            // Write packet to TX FIFO
            cc11xx_WriteTXFIFO(str_packet, sizeof(str_packet)-1);

            // Enable TX (Command strobe)
            cc11xx_CmdStrobe(CC11XX_STX);
            
            // Disable the switch and the PA
            rf6886_Disable();
            rf_switch_Disable();
        }
        
        uint8_t beacon_pkt_interval_sec_counter = timer_sec_counter;
        while((uint8_t)((timer_sec_counter - beacon_pkt_interval_sec_counter) % 60) < BEACON_PKT_PERIOD_SEC)
        {
#if DEBUG_MODE == false
            WDT_A_resetTimer(WDT_A_BASE);
#endif // DEBUG_MODE

            // Enter LPM1 with interrupts enabled
            _BIS_SR(LPM1_bits + GIE);
        }
    }
}

//! \} End of beacon group
