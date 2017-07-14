/*
 * rf4463.c
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina
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
 * \file rf4463f30.c
 * 
 * \brief NiceRF RF4463 driver implementation.
 * 
 * This library suits for RF4463PRO and RF4463F30 in FIFO mode.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 01/06/2017
 * 
 * \addtogroup rf4463
 * \{
 */

#include <libs/driverlib/driverlib.h>
#include <modules/time/delay.h>         // delay_ms()

#include "rf4463.h"
#include "rf4463_pinmap.h"
#include "rf4463_config.h"
#include "rf4463_registers.h"
#include "rf4463_reg_config.h"

const uint8_t RF4463_CONFIGURATION_DATA[] = RADIO_CONFIGURATION_DATA_ARRAY;

uint8_t rf4463_init()
{
    rf4463_gpio_init();
    rf4463_turn_off_pa();
    
    if (rf4463_spi_init() == STATUS_FAIL)
    {
        return STATUS_FAIL;
    }
    
    // Reset the RF4463
    rf4463_reset();
    
    // Registers configuration
    rf4463_reg_config();
    
    // Set max. TX power
    rf4463_set_tx_power(127);
    
    // Check if the RF4463 is working
    if (!rf4463_check_device())
    {
        return STATUS_FAIL;
    }
    
    return STATUS_SUCCESS;
}

static void rf4463_gpio_init()
{
    GPIO_setAsOutputPin(RF4463_SDN_PORT, RF4463_SDN_PIN);
    GPIO_setAsInputPin(RF4463_nIRQ_PORT, RF4463_nIRQ_PIN);
    
    GPIO_setAsOutputPin(RF4463_GPIO0_PORT, RF4463_GPIO0_PIN);
    GPIO_setAsOutputPin(RF4463_GPIO0_PORT, RF4463_GPIO1_PIN);
}

static uint8_t rf4463_spi_init()
{
    // MISO, MOSI and SCLK init.
    GPIO_setAsPeripheralModuleFunctionInputPin(RF4463_SPI_PORT, RF4463_SDI_PIN + RF4463_SDO_PIN + RF4463_SCLK_PIN);
    
    // CSn init.
    GPIO_setAsOutputPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
	GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    // Config. SPI as Master
#if RF4463_SPI_USCI == USCI_A
    USCI_A_SPI_initMasterParam spi_params = {0};
    spi_params.selectClockSource     = USCI_A_SPI_CLOCKSOURCE_SMCLK;
    spi_params.msbFirst              = USCI_A_SPI_MSB_FIRST;
    spi_params.clockPhase            = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_params.clockPolarity         = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
#elif RF4463_SPI_USCI == USCI_B
    USCI_B_SPI_initMasterParam spi_params = {0};
    spi_params.selectClockSource     = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    spi_params.msbFirst              = USCI_B_SPI_MSB_FIRST;
    spi_params.clockPhase            = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_params.clockPolarity         = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
#endif // RF4463_SPI_USCI
    spi_params.clockSourceFrequency  = UCS_getSMCLK();
    spi_params.desiredSpiClock       = RF4463_SPI_CLK;

    // SPI initialization
#if RF4463_SPI_USCI == USCI_A
    if (USCI_A_SPI_initMaster(RF4463_SPI_BASE_ADDRESS, &spi_params) == STATUS_SUCCESS)
    {
        USCI_A_SPI_enable(RF4463_SPI_BASE_ADDRESS);
#elif RF4463_SPI_USCI == USCI_B
    if (USCI_B_SPI_initMaster(RF4463_SPI_BASE_ADDRESS, &spi_params) == STATUS_SUCCESS)
    {
        USCI_B_SPI_enable(RF4463_SPI_BASE_ADDRESS);
#endif // RF4463_SPI_USCI

        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

static void rf4463_reg_config()
{
    // Set RF parameter like frequency, data rate, etc.
    rf4463_set_config(RF4463_CONFIGURATION_DATA, sizeof(RF4463_CONFIGURATION_DATA));
    
    uint8_t buf[20];
    
    // Set antenna switch. In RF4463 it is GPIO2 and GPIO3 (Don't change settings of GPIO2, GPIO3, NIRQ and SDO)
    buf[0] = RF4463_GPIO_NO_CHANGE;
    buf[1] = RF4463_GPIO_NO_CHANGE;
    buf[2] = RF4463_GPIO_RX_STATE;
    buf[3] = RF4463_GPIO_TX_STATE;
    buf[4] = RF4463_NIRQ_INTERRUPT_SIGNAL;
    buf[5] = RF4463_GPIO_SPI_DATA_OUT;
    rf4463_set_cmd(6, RF4463_CMD_GPIO_PIN_CFG, buf);
    
    // Frequency adjust (Frequency will be inaccurate if you change this parameter)
    buf[0] = 98;
    rf4463_set_properties(RF4463_PROPERTY_GLOBAL_XO_TUNE, 1, buf);
    
    // TX = RX = 64 byte, PH mode, high performance mode
    buf[0] = 0x40;
    rf4463_set_properties(RF4463_PROPERTY_GLOBAL_CONFIG, 1, buf);
    
    // Set preamble
    buf[0] = 0x08;      // 8 bytes Preamble			
    buf[1] = 0x14;      // detect 20 bits
    buf[2] = 0x00;			
    buf[3] = 0x0F;
    buf[4] = RF4463_PREAMBLE_FIRST_1 | RF4463_PREAMBLE_LENGTH_BYTES | RF4463_PREAMBLE_STANDARD_1010;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
    buf[8] = 0x00;
    rf4463_set_properties(RF4463_PROPERTY_PREAMBLE_TX_LENGTH, 9, buf);
    
    // Set sync. words
    buf[0] = 0x2D;
    buf[1] = 0xD4;
    rf4463_set_sync_word(buf, 2);
    
    // Set CRC
    buf[0] = RF4463_CRC_SEED_ALL_1S|RF4463_CRC_ITU_T ;			
    rf4463_set_properties(RF4463_PROPERTY_PKT_CRC_CONFIG, 1, buf);
    
    buf[0] = RF4463_CRC_ENDIAN;
    rf4463_set_properties(RF4463_PROPERTY_PKT_CONFIG1, 1, buf);

    buf[0] = RF4463_IN_FIFO|RF4463_DST_FIELD_ENUM_2;
    buf[1] = RF4463_SRC_FIELD_ENUM_1;
    buf[2] = 0x00;
    rf4463_set_properties(RF4463_PROPERTY_PKT_LEN, 3, buf);

    // Set length of Field 1 -- 4
    // Variable len, field as length field, field 2 as data field
    // Didn't use field 3 -- 4
    buf[0] = 0x00;
    buf[1] = 0x01;
    buf[2] = RF4463_FIELD_CONFIG_PN_START;
    buf[3] = RF4463_FIELD_CONFIG_CRC_START | RF4463_FIELD_CONFIG_SEND_CRC | RF4463_FIELD_CONFIG_CHECK_CRC | RF4463_FIELD_CONFIG_CRC_ENABLE;
    buf[4] = 0x00;
    buf[5] = 50;
    buf[6] = RF4463_FIELD_CONFIG_PN_START;
    buf[7] = RF4463_FIELD_CONFIG_CRC_START | RF4463_FIELD_CONFIG_SEND_CRC | RF4463_FIELD_CONFIG_CHECK_CRC | RF4463_FIELD_CONFIG_CRC_ENABLE;
    buf[8] = 0x00;
    buf[9] = 0x00;
    buf[10] = 0x00;
    buf[11] = 0x00;
    rf4463_set_properties(RF4463_PROPERTY_PKT_FIELD_1_LENGTH_12_8, 12, buf);

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
    rf4463_set_properties(RF4463_PROPERTY_PKT_FIELD_4_LENGTH_12_8, 8, buf);
}

void rf4463_reset()
{
    uint8_t buffer[] = {RF_POWER_UP};
    
    GPIO_setOutputHighOnPin(RF4463_SDN_PORT, RF4463_SDN_PIN);
    delay_ms(100);
    GPIO_setOutputLowOnPin(RF4463_SDN_PORT, RF4463_SDN_PIN);
    delay_ms(20);           // Wait for RF4463 stabilization
    
    // Send power-up command
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write(buffer, sizeof(buffer));
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    delay_ms(200);
}

bool rf4463_tx_packet(uint8_t *send_buf, uint8_t send_len)
{
    rf4463_fifo_reset();        // Clear FIFO
    rf4463_write_tx_fifo(send_buf, send_len);
    rf4463_set_tx_interrupt();
    rf4463_clear_interrupts();
    rf4463_enter_tx_mode();
    
    uint16_t tx_timer = RF4463_TX_TIMEOUT;
    
    while(tx_timer--)
    {
        if (rf4463_wait_nIRQ())         // Wait "interruption"
        {
            return true;
        }
        
        delay_ms(1);
    }
    
    rf4463_init();
    
    return false;
}

uint8_t rf4463_rx_packet(uint8_t *rx_buf)
{
    uint8_t rx_len = rf4463_read_rx_fifo(rx_buf);   // Read data from the FIFO
    rf4463_fifo_reset();                            // Clear FIFO
    
    return rx_len;
}

bool rf4463_rx_init()
{
    uint8_t length = 50;
    
    rf4463_set_properties(RF4463_PROPERTY_PKT_FIELD_2_LENGTH_7_0, sizeof(length), &length);     // Reload RX FIFO size
    rf4463_fifo_reset();        // Clear FIFO
    rf4463_set_rx_interrupt();
    rf4463_clear_interrupts();
    rf4463_enter_rx_mode();
    
    return true;
}

void rf4463_spi_write_byte(uint8_t byte)
{
#if RF4463_SPI_USCI == USCI_A
    USCI_A_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
    USCI_A_SPI_transmitData(RF4463_SPI_BASE_ADDRESS, byte);
#elif RF4463_SPI_USCI == USCI_B
    USCI_B_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_transmitData(RF4463_SPI_BASE_ADDRESS, byte);
#endif // RF4463_SPI_USCI
}

void rf4463_spi_write(uint8_t *data, uint16_t size)
{
    while(size--)
    {
        rf4463_spi_write_byte(*data++);
    }
}

uint8_t rf4463_spi_read_byte()
{
#if RF4463_SPI_USCI == USCI_A
    // Wait until new data was written into RX buffer
    while(!USCI_A_SPI_getInterruptStatus(RF4463_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
    {
        
    }
    
    USCI_A_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
    
    return USCI_A_SPI_receiveData(RF4463_SPI_BASE_ADDRESS);
#elif RF4463_SPI_USCI == USCI_B
    // Wait until new data was written into RX buffer
    while(!USCI_B_SPI_getInterruptStatus(RF4463_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
    {
        
    }
    
    USCI_B_SPI_clearInterrupt(RF4463_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
    
    return USCI_B_SPI_receiveData(RF4463_SPI_BASE_ADDRESS);
#endif // RF4463_SPI_USCI
}

void rf4463_spi_read(uint8_t *data, uint16_t size)
{
    while(size--)
    {
        *data++ = rf4463_spi_read_byte();
    }
}

bool rf4463_check_device()
{
    uint8_t buffer[9];
    uint16_t part_info;
    
    if (!rf4463_get_cmd(9, RF4463_CMD_PART_INFO, buffer))
    {
        return false;
    }
    
    part_info = buffer[2] << 8 | buffer[3];
    if (part_info != RF4463_PART_INFO)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool rf4463_check_cts()
{
    uint16_t timeout_counter;
    timeout_counter = RF4463_CTS_TIMEOUT;
    
    while(timeout_counter--)
    {
        GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
        
        rf4463_spi_write_byte(RF4463_CMD_READ_BUF);
        
        if (rf4463_spi_read_byte() == RF4463_CTS_REPLY) // Read CTS
        {
            GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
            return true;
        }
        
        GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    }
    
    return	false;
}

bool rf4463_get_cmd(uint8_t length, uint8_t cmd, uint8_t *para_buf)
{
    if (!rf4463_check_cts())
    {
        return false;
    }
    
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write_byte(cmd);    // Send the read command
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    // Check if the RF4463 is ready
    if (!rf4463_check_cts())
    {
        return false;
    }
    
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write_byte(RF4463_CMD_READ_BUF);     // Turn to read command mode
    rf4463_spi_read(para_buf, length);              // Read the parameters
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    return true;
}

bool rf4463_set_tx_power(uint8_t pwr)
{
    if (pwr > 127)      // Max. value is 127
    {
        return false;
    }
    
    uint8_t buffer[4] = {0x08, 0x00, 0x00, 0x3D};
    buffer[1] = pwr;
    
    return rf4463_set_properties(RF4463_PROPERTY_PA_MODE, sizeof(buffer), buffer);
}

bool rf4463_set_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf)
{
    uint8_t buffer[4];
    
    if (!rf4463_check_cts())
    {
        return false;
    }
    
    buffer[0] = RF4463_CMD_SET_PROPERTY;
    buffer[1] = start_property >> 8;        // GROUP
    buffer[2] = length;                     // NUM_PROPS
    buffer[3] = start_property & 0xFF;      // START_PROP
    
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write(buffer, 4);            // Set start property and read length
    rf4463_spi_write(para_buf, length);     // Set parameters
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    return true;
}

bool rf4463_get_properties(uint16_t start_property, uint8_t length, uint8_t *para_buf)
{
    if (!rf4463_check_cts())
    {
        return false;
    }
    
    uint8_t buffer[4];
    buffer[0] = RF4463_CMD_GET_PROPERTY;
    buffer[1] = start_property >> 8;    // GROUP
    buffer[2] = length;                 // NUM_PROPS
    buffer[3] = start_property & 0xFF;  // START_PROP
    
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write(buffer, 4);        // Set start property and read length
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    if (!rf4463_check_cts())
    {
        return false;
    }
    
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write_byte(RF4463_CMD_READ_BUF);     // Turn to read command mode
    rf4463_spi_write(para_buf, length);             // Read parameters
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    return true;
}

void rf4463_set_config(const uint8_t *parameters, uint16_t para_len)
{
    uint8_t cmd_len;
    uint16_t cmd;
    uint16_t pos;
    uint8_t buffer[30];
    
    para_len--;
    cmd_len = parameters[0];
    pos = cmd_len + 1;
    
    while(pos < para_len)
    {
        cmd_len = parameters[pos++] - 1;            // Get command len
        cmd = parameters[pos++];                    // Get command
        memcpy(buffer, parameters + pos, cmd_len);  // Get parameters
        
        rf4463_set_cmd(cmd_len, cmd, buffer);
        pos += cmd_len;
    }
}

bool rf4463_set_preamble_len(uint8_t len)
{
    return rf4463_set_properties(RF4463_PROPERTY_PREAMBLE_TX_LENGTH, 1, &len);
}

bool rf4463_set_sync_word(uint8_t *sync_word, uint8_t len)
{
    if ((len == 0) || (len > 3))
    {
        return false;
    }
    
    uint8_t buffer[5];
    buffer[0] = len - 1;
    memcpy(buffer + 1, sync_word, len);
    
    return rf4463_set_properties(RF4463_PROPERTY_SYNC_CONFIG, sizeof(buffer), buffer);
}

bool rf4463_set_gpio_mode(uint8_t gpio0_mode, uint8_t gpio1_mode)
{
    uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    buffer[0] = gpio0_mode;
    buffer[1] = gpio1_mode;
    
    return rf4463_set_cmd(sizeof(buffer), RF4463_CMD_GPIO_PIN_CFG, buffer);
}

bool rf4463_set_cmd(uint8_t len, uint8_t cmd, uint8_t *para_buf)
{
    if (!rf4463_check_cts())
    {
        return false;
    }
    
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write_byte(cmd);         // Send the command
    rf4463_spi_write(para_buf, len);    // Send the parameters
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    return true;
}

bool rf4463_set_tx_interrupt()
{
    uint8_t buffer[3] = {0x01, 0x20, 0x00};     // Enable PACKET_SENT interruption
    
    return rf4463_set_properties(RF4463_PROPERTY_INT_CTL_ENABLE, 3, buffer);
}

bool rf4463_set_rx_interrupt()
{
    uint8_t buffer[3] = {0x03, 0x18, 0x00};     // Enable PACKET_RX interrution
    
    return rf4463_set_properties(RF4463_PROPERTY_INT_CTL_ENABLE, 3, buffer);
}

bool rf4463_clear_interrupts()
{
    uint8_t buffer[] = {0x00, 0x00, 0x00};
    
    return rf4463_set_cmd(sizeof(buffer), RF4463_CMD_GET_INT_STATUS, buffer);
}

void rf4463_write_tx_fifo(uint8_t *data, uint8_t len)
{
    
    rf4463_set_properties(RF4463_PROPERTY_PKT_FIELD_2_LENGTH_7_0, sizeof(len), &len);
    uint8_t buffer[len+1];
    buffer[0] = len;
    memcpy(buffer+1, data, len);
    rf4463_set_cmd(len+1, RF4463_CMD_TX_FIFO_WRITE, buffer);
}

uint8_t rf4463_read_rx_fifo(uint8_t *data)
{
    if (!rf4463_check_cts())
    {
        return 0;
    }
    
    uint8_t read_len;
    GPIO_setOutputLowOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    rf4463_spi_write_byte(RF4463_CMD_RX_FIFO_READ);
    rf4463_spi_read(&read_len, 1);
    rf4463_spi_read(data, read_len);
    GPIO_setOutputHighOnPin(RF4463_NSEL_PORT, RF4463_NSEL_PIN);
    
    return read_len;
}

void rf4463_fifo_reset()
{
    uint8_t data = 0x03;
    
    rf4463_set_cmd(sizeof(data), RF4463_CMD_FIFO_INFO, &data);
}

void rf4463_enter_tx_mode()
{
    uint8_t buffer[] = {0x00, 0x30, 0x00, 0x00};
    
    buffer[0] = RF4463_FREQ_CHANNEL;
    
    rf4463_set_cmd(4, RF4463_CMD_START_TX, buffer);
}

void rf4463_enter_rx_mode()
{
    uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08};
    
    buffer[0] = RF4463_FREQ_CHANNEL;
    
    rf4463_set_cmd(7, RF4463_CMD_START_RX, buffer);
}

bool rf4463_enter_standby_mode()
{
    uint8_t data = 0x01;
    
    return rf4463_set_cmd(1, RF4463_CMD_CHANGE_STATE, &data);
}

bool rf4463_wait_nIRQ()
{
    if (GPIO_getInputPinValue(RF4463_nIRQ_PORT, RF4463_nIRQ_PIN) == GPIO_INPUT_PIN_HIGH)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void rf4463_turn_on_pa()
{
    GPIO_setOutputLowOnPin(RF4463_SDN_PORT, RF4463_SDN_PIN);
}

void rf4463_turn_off_pa()
{
    GPIO_setOutputHighOnPin(RF4463_SDN_PORT, RF4463_SDN_PIN);
}

//! \} End of rf4463 group
