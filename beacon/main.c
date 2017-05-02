/*
 * main.c
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
 * \file main.c
 * 
 * \brief Main file.
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
#include "ngham/ngham.h"

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
#include "inc/delay.h"

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

    // Protocols initialization
    ngham_Init();
    NGHam_TX_Packet ngham_packet;
    uint8_t ngham_str_pkt[NGH_MAX_TOT_SIZE];
    uint16_t ngham_str_pkt_len = 0;
    
    AX25_Packet ax25_packet;
    uint8_t ax25_str_pkt[AX25_FLORIPASAT_HEADER_SIZE + PKT_PAYLOAD_LEN + 1];

    // Payload initialization
    uint8_t pkt_payload[PKT_PAYLOAD_LEN + 1];

    // Status LED initialization
    led_Init();

#if DEBUG_MODE == true
    debug_PrintMsg("Running...\n");
#endif // DEBUG_MODE

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
            if (obdh_gpio_obdh_state == OBDH_GPIO_OBDH_TX_OFF)
            {
                pkt_payload_Gen(pkt_payload, eps_data);
                
                ngham_TxPktGen(&ngham_packet, pkt_payload, PKT_PAYLOAD_LEN);
                ngham_Encode(&ngham_packet, ngham_str_pkt, &ngham_str_pkt_len);
                
                ax25_BeaconPacketGen(&ax25_packet, pkt_payload, PKT_PAYLOAD_LEN);
                ax25_Packet2String(&ax25_packet, ax25_str_pkt, sizeof(ax25_str_pkt)-1);
                
                // Flush the TX FIFO
                cc11xx_CmdStrobe(CC11XX_SFTX);
                
                // Enable the switch and the PA
                rf6886_Enable();
                rf_switch_Enable();

                // Write AX25 packet to TX FIFO and enable TX
                cc11xx_WriteTXFIFO(ax25_str_pkt, sizeof(ax25_str_pkt)-1);
                obdh_GPIO_SendToOBDH(OBDH_GPIO_BEACON_TX_ON);
                cc11xx_CmdStrobe(CC11XX_STX);
                
                // Disable the switch and the PA
                rf6886_Disable();
                rf_switch_Disable();
                
                // Wait for AX25 packet transmission to end
                delay_ms((uint16_t)(1.2*sizeof(ax25_str_pkt)*1000/1200/8));
                
                // Flush the TX FIFO
                cc11xx_CmdStrobe(CC11XX_SFTX);
                
                // Enable the switch and the PA
                rf6886_Enable();
                rf_switch_Enable();
                
                // Write NGHam packet to TX FIFO and enable TX
                cc11xx_WriteTXFIFO(ngham_str_pkt, ngham_str_pkt_len);
                cc11xx_CmdStrobe(CC11XX_STX);
                
                // Wait for NGHam packet transmission to end
                delay_ms((uint16_t)(1.2*ngham_str_pkt_len*1000/1200/8));
                obdh_GPIO_SendToOBDH(OBDH_GPIO_BEACON_TX_OFF);
                
                // Disable the switch and the PA
                rf6886_Disable();
                rf_switch_Disable();
            }
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
