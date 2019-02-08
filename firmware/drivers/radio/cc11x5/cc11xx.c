/*
 * cc11xx.c
 * 
 * Copyright (C) 2017, Universidade Federal de Santa Catarina.
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
 * \brief CC1125/CC1175 functions implementation
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 *
 * \addtogroup cc11xx
 * \{
 */

#include "cc11xx.h"
#include "cc11xx_floripasat_reg_config.h"

uint8_t cc11xx_Init()
{
    // SPI initialization
    if (cc11xx_SPI_Init() == STATUS_SUCCESS)
    {
        cc11xx_GPIO_Init();

        cc11xx_ManualReset();

        cc11xx_RegConfig();

        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

void cc11xx_RegConfig()
{
    uint16_t i;
    uint8_t write_byte;

    // Write registers settings to radio (Values in "cc11xx_floripasat_reg_config.h")
    for(i=0; i<(sizeof(reg_values)/sizeof(RegistersSettings)); i++)
    {
        write_byte = reg_values[i].data;
        cc11xx_WriteReg(reg_values[i].addr, &write_byte, sizeof(reg_values[i].data));
    }
}

uint8_t cc11xx_WriteReg(uint16_t addr, uint8_t *pData, uint8_t len)
{
    uint8_t temp_ext  = (uint8_t)(addr >> 8);
    uint8_t temp_addr = (uint8_t)(addr & 0x00FF);
    uint8_t chip_status = 0;

    // Checking if this is a FIFO access (if true, returns chip not ready)
    if ((CC11XX_SINGLE_TXFIFO <= temp_addr) && (temp_ext == 0))
    {
        return CC11XX_STATUS_CHIP_RDYn_H;
    }

    // Decide what register space is accessed
    if (!temp_ext)
    {
        chip_status = cc11xx_8BitRegAccess((CC11XX_BURST_ACCESS | CC11XX_WRITE_ACCESS), temp_addr, pData, len);
    }
    else if (temp_ext == 0x2F)  // Extended addresses space
    {
        chip_status = cc11xx_16BitRegAccess((CC11XX_BURST_ACCESS | CC11XX_WRITE_ACCESS), temp_ext, temp_addr, pData, len);
    }

    return chip_status;
}

uint8_t cc11xx_ReadReg(uint16_t addr, uint8_t *pData, uint8_t len)
{
    uint8_t temp_ext  = (uint8_t)(addr >> 8);
    uint8_t temp_addr = (uint8_t)(addr & 0x00FF);
    uint8_t chip_status = 0;

    // Checking if this is a FIFO access (if true, returns chip not ready)
    if ((CC11XX_SINGLE_TXFIFO <= temp_addr) && (temp_ext == 0))
    {        
        return CC11XX_STATUS_CHIP_RDYn_H;
    }

    // Decide what register space is accessed
    if (!temp_ext)
    {
        chip_status = cc11xx_8BitRegAccess((CC11XX_BURST_ACCESS | CC11XX_READ_ACCESS), temp_addr, pData, len);
    }
    else if (temp_ext == 0x2F)  // Extended addresses space
    {
        chip_status = cc11xx_16BitRegAccess((CC11XX_BURST_ACCESS | CC11XX_READ_ACCESS), temp_ext, temp_addr, pData, len);
    }

    return chip_status;
}

uint8_t cc11xx_CmdStrobe(uint8_t cmd)
{
    uint8_t chip_status;

    GPIO_setOutputLowOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);    // CSn = 0

    while(GPIO_getInputPinValue(CC11XX_MISO_PORT, CC11XX_MISO_PIN) == GPIO_INPUT_PIN_HIGH)
    {
        
    }

#if CC11XX_SPI_USCI == USCI_A
    USCI_A_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
    USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, cmd);

    // Wait until new data was written into RX buffer
    while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
    {
        
    }

    chip_status = USCI_A_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
#elif CC11XX_SPI_USCI == USCI_B
    USCI_B_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, cmd);

    // Wait until new data was written into RX buffer
    while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
    {
        
    }

    chip_status = USCI_B_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
#endif // CC11XX_SPI_USCI

    GPIO_setOutputHighOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);   // CSn = 1

    return chip_status;
}

uint8_t cc11xx_8BitRegAccess(uint8_t access_type, uint8_t addr_byte, uint8_t *pData, uint16_t len)
{
	uint8_t read_value = 0;
    uint8_t header_byte = access_type | addr_byte;

	// Pull CSn low
	GPIO_setOutputLowOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);
    
    // Wait for MISO (or SO) to go low before communication starts
    while(GPIO_getInputPinValue(CC11XX_MISO_PORT, CC11XX_MISO_PIN) == GPIO_INPUT_PIN_HIGH)
    {
        
    }

    // Send register address byte
#if CC11XX_SPI_USCI == USCI_A
    USCI_A_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, header_byte);
#elif CC11XX_SPI_USCI == USCI_B
    USCI_B_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, header_byte);
#endif // CC11XX_SPI_USCI

    // Wait until new data was written into RX buffer
#if CC11XX_SPI_USCI == USCI_A
    while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT));
#elif CC11XX_SPI_USCI == USCI_B
    while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT));
#endif // CC11XX_SPI_USCI

    // Storing chip status
#if CC11XX_SPI_USCI == USCI_A
    read_value = USCI_A_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
#elif CC11XX_SPI_USCI == USCI_B
    read_value = USCI_B_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
#endif // CC11XX_SPI_USCI
    
	cc11xx_ReadWriteBurstSingle(header_byte, pData, len);
	
    // CSn = high after transfers
    GPIO_setOutputHighOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);

    // Return the status byte value
	return read_value;
}

void cc11xx_ReadWriteBurstSingle(uint8_t header, uint8_t *pData, uint16_t len)
{
    uint16_t i;
    
    // Communicate len number of bytes: if RX - the procedure sends 0x00 to push bytes from slave
    if (header & CC11XX_READ_ACCESS)
    {
        if (header & CC11XX_BURST_ACCESS)
        {
            for(i=0; i<len; i++)
            {
#if CC11XX_SPI_USCI == USCI_A
                // Wait until new data was written into RX buffer
                while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
                {
                    
                }
                USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, 0);
                
                // Wait until new data was written into RX buffer
                while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
                {
                    
                }

                *pData = USCI_A_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);  // Store pData from last pData RX
#elif CC11XX_SPI_USCI == USCI_B
                // Wait until new data was written into RX buffer
                while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
                {
                    
                }
                USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, 0);
                
                // Wait until new data was written into RX buffer
                while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
                {
                    
                }

                *pData = USCI_B_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);  // Store pData from last pData RX
#endif // CC11XX_SPI_USCI

                pData++;
            }
        }
        else    // CC11XX_SINGLE_ACCESS
        {
#if CC11XX_SPI_USCI == USCI_A
            USCI_A_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
            USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, 0);

            // Wait until new data was written into RX buffer
            while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
            {
                
            }
            
            *pData = USCI_A_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
#elif CC11XX_SPI_USCI == USCI_B
            USCI_B_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
            USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, 0);

            // Wait until new data was written into RX buffer
            while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
            {
                
            }
            
            *pData = USCI_B_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
#endif // CC11XX_SPI_USCI
        }
    }
    else    // CC11XX_WRITE_ACCESS
    {
        if (header & CC11XX_BURST_ACCESS)
        {
            // Communicate len number of bytes: if TX - the procedure doesn't overwrite pData
            for(i=0; i<len; i++)
            {
#if CC11XX_SPI_USCI == USCI_A
                USCI_A_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
                USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, *pData);                
                
                // Wait until new data was written into RX buffer
                while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
                {
                    
                }
#elif CC11XX_SPI_USCI == USCI_B
                USCI_B_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
                USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, *pData);                
                
                // Wait until new data was written into RX buffer
                while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
                {
                    
                }
#endif // CC11XX_SPI_USCI
                pData++;
            }
        }
        else    // CC11XX_SINGLE_ACCESS
        {
#if CC11XX_SPI_USCI == USCI_A
            USCI_A_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
            USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, *pData);

            // Wait until new data was written into RX buffer
            while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
            {
                
            }
#elif CC11XX_SPI_USCI == USCI_B
            USCI_B_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
            USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, *pData);

            // Wait until new data was written into RX buffer
            while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
            {
                
            }
#endif // CC11XX_SPI_USCI
        }
    }
}

uint8_t cc11xx_16BitRegAccess(uint8_t access_type, uint8_t ext_addr, uint8_t reg_addr, uint8_t *pData, uint8_t len)
{
    uint8_t read_value = 0;
    uint8_t header_byte = access_type | ext_addr;

    // Pull CSn low to start transfer
    GPIO_setOutputLowOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);
    
    // Wait for MISO (or SO) to go low before communication starts
    while(GPIO_getInputPinValue(CC11XX_MISO_PORT, CC11XX_MISO_PIN) == GPIO_INPUT_PIN_HIGH)
    {
        
    }
    
#if CC11XX_SPI_USCI == USCI_A
    // Send extended address byte with access type bits set
    USCI_A_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
    USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, header_byte);
    
    // Wait until new data was written into RX buffer
    while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
    {
        
    }
    
    // Storing chip status
    read_value = USCI_A_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
    
    USCI_A_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT);
    USCI_A_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, reg_addr);
    
    // Wait until new data was written into RX buffer
    while(!USCI_A_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_A_SPI_RECEIVE_INTERRUPT))
    {
        
    }
#elif CC11XX_SPI_USCI == USCI_B
    // Send extended address byte with access type bits set
    USCI_B_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, header_byte);
    
    // Wait until new data was written into RX buffer
    while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
    {
        
    }
    
    // Storing chip status
    read_value = USCI_B_SPI_receiveData(CC11XX_SPI_BASE_ADDRESS);
    
    USCI_B_SPI_clearInterrupt(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_transmitData(CC11XX_SPI_BASE_ADDRESS, reg_addr);
    
    // Wait until new data was written into RX buffer
    while(!USCI_B_SPI_getInterruptStatus(CC11XX_SPI_BASE_ADDRESS, USCI_B_SPI_RECEIVE_INTERRUPT))
    {
        
    }
#endif // CC11XX_SPI_USCI
    
    // Communicate len number of bytes
    cc11xx_ReadWriteBurstSingle(header_byte, pData, len);
    
    // Pull CSn high after transfer
    GPIO_setOutputHighOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);

    // Return the status byte value
    return read_value;
}

void cc11xx_ManualReset()
{
    GPIO_setOutputLowOnPin(CC11XX_RESET_PORT, CC11XX_RESET_PIN);
    __delay_cycles(100);
    GPIO_setOutputHighOnPin(CC11XX_RESET_PORT, CC11XX_RESET_PIN);
}

void cc11xx_ManualCalibration()
{
    uint8_t original_fs_cal2;
    uint8_t calResults_for_vcdac_start_high[3];
    uint8_t calResults_for_vcdac_start_mid[3];
    uint8_t marcstate;
    uint8_t write_byte;

    // 1) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    write_byte = 0x00;
    cc11xx_WriteReg(CC11XX_FS_VCO2, &write_byte, 1);

    // 2) Start with high VCDAC (original VCDAC_START + 2):
    cc11xx_ReadReg(CC11XX_FS_CAL2, &original_fs_cal2, 1);
    write_byte = original_fs_cal2 + CC11XX_VCDAC_START_OFFSET;
    cc11xx_WriteReg(CC11XX_FS_CAL2, &write_byte, 1);

    // 3) Calibrate and wait for calibration to be done (radio back in IDLE state)
    cc11xx_CmdStrobe(CC11XX_SCAL);

    do
    {
        cc11xx_ReadReg(CC11XX_MARCSTATE, &marcstate, 1);
    }
    while(marcstate != 0x41);

    // 4) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with high VCDAC_START value
    cc11xx_ReadReg(CC11XX_FS_VCO2, &calResults_for_vcdac_start_high[CC11XX_FS_VCO2_INDEX], 1);
    cc11xx_ReadReg(CC11XX_FS_VCO4, &calResults_for_vcdac_start_high[CC11XX_FS_VCO4_INDEX], 1);
    cc11xx_ReadReg(CC11XX_FS_CHP, &calResults_for_vcdac_start_high[CC11XX_FS_CHP_INDEX], 1);

    // 5) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    write_byte = 0x00;
    cc11xx_WriteReg(CC11XX_FS_VCO2, &write_byte, 1);

    // 6) Continue with mid VCDAC (original VCDAC_START):
    write_byte = original_fs_cal2;
    cc11xx_WriteReg(CC11XX_FS_CAL2, &write_byte, 1);

    // 7) Calibrate and wait for calibration to be done (radio back in IDLE state)
    cc11xx_CmdStrobe(CC11XX_SCAL);

    do
    {
        cc11xx_ReadReg(CC11XX_MARCSTATE, &marcstate, 1);
    }
    while(marcstate != 0x41);

    // 8) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with mid VCDAC_START value
    cc11xx_ReadReg(CC11XX_FS_VCO2, &calResults_for_vcdac_start_mid[CC11XX_FS_VCO2_INDEX], 1);
    cc11xx_ReadReg(CC11XX_FS_VCO4, &calResults_for_vcdac_start_mid[CC11XX_FS_VCO4_INDEX], 1);
    cc11xx_ReadReg(CC11XX_FS_CHP, &calResults_for_vcdac_start_mid[CC11XX_FS_CHP_INDEX], 1);

    // 9) Write back highest FS_VCO2 and corresponding FS_VCO and FS_CHP result
    if (calResults_for_vcdac_start_high[CC11XX_FS_VCO2_INDEX] > calResults_for_vcdac_start_mid[CC11XX_FS_VCO2_INDEX])
    {
        write_byte = calResults_for_vcdac_start_high[CC11XX_FS_VCO2_INDEX];
        cc11xx_WriteReg(CC11XX_FS_VCO2, &write_byte, 1);
        write_byte = calResults_for_vcdac_start_high[CC11XX_FS_VCO4_INDEX];
        cc11xx_WriteReg(CC11XX_FS_VCO4, &write_byte, 1);
        write_byte = calResults_for_vcdac_start_high[CC11XX_FS_CHP_INDEX];
        cc11xx_WriteReg(CC11XX_FS_CHP, &write_byte, 1);
    }
    else
    {
        write_byte = calResults_for_vcdac_start_mid[CC11XX_FS_VCO2_INDEX];
        cc11xx_WriteReg(CC11XX_FS_VCO2, &write_byte, 1);
        write_byte = calResults_for_vcdac_start_mid[CC11XX_FS_VCO4_INDEX];
        cc11xx_WriteReg(CC11XX_FS_VCO4, &write_byte, 1);
        write_byte = calResults_for_vcdac_start_mid[CC11XX_FS_CHP_INDEX];
        cc11xx_WriteReg(CC11XX_FS_CHP, &write_byte, 1);
    }
}

uint8_t cc11xx_WriteTXFIFO(uint8_t *pData, uint8_t len)
{
    uint8_t chip_status = cc11xx_8BitRegAccess(CC11XX_WRITE_ACCESS, CC11XX_BURST_TXFIFO, pData, len);

    return chip_status;
}

uint8_t cc11xx_SPI_Init()
{
    // MISO, MOSI and SCLK init.
    GPIO_setAsPeripheralModuleFunctionInputPin(CC11XX_SPI_PORT, CC11XX_MISO_PIN + CC11XX_MOSI_PIN + CC11XX_SCLK_PIN);
    
    // CSn init.
    GPIO_setAsOutputPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);
	GPIO_setOutputHighOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);   // CSn must be kept low during SPI transfers
    
    // Config. SPI as Master
#if CC11XX_SPI_USCI == USCI_A
    USCI_A_SPI_initMasterParam spi_params = {0};
    spi_params.selectClockSource     = USCI_A_SPI_CLOCKSOURCE_SMCLK;
    spi_params.msbFirst              = USCI_A_SPI_MSB_FIRST;
    spi_params.clockPhase            = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_params.clockPolarity         = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
#elif CC11XX_SPI_USCI == USCI_B
    USCI_B_SPI_initMasterParam spi_params = {0};
    spi_params.selectClockSource     = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    spi_params.msbFirst              = USCI_B_SPI_MSB_FIRST;
    spi_params.clockPhase            = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_params.clockPolarity         = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
#endif // CC11XX_SPI_USCI
    spi_params.clockSourceFrequency  = UCS_getSMCLK();
    spi_params.desiredSpiClock       = CC11XX_SPI_CLK;

    // SPI initialization
#if CC11XX_SPI_USCI == USCI_A
    if (USCI_A_SPI_initMaster(CC11XX_SPI_BASE_ADDRESS, &spi_params) == STATUS_SUCCESS)
    {
        USCI_A_SPI_enable(CC11XX_SPI_BASE_ADDRESS);
#elif CC11XX_SPI_USCI == USCI_B
    if (USCI_B_SPI_initMaster(CC11XX_SPI_BASE_ADDRESS, &spi_params) == STATUS_SUCCESS)
    {
        USCI_B_SPI_enable(CC11XX_SPI_BASE_ADDRESS);
#endif // CC11XX_SPI_USCI

        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

void cc11xx_GPIO_Init()
{
    // Reset pin
    GPIO_setAsOutputPin(CC11XX_RESET_PORT, CC11XX_RESET_PIN);
    
    // CC11xx GPIO0
    GPIO_setAsOutputPin(CC11XX_GPIO0_PORT, CC11XX_GPIO0_PIN);

    // CC11xx GPIO2
    GPIO_setAsOutputPin(CC11XX_GPIO2_PORT, CC11XX_GPIO2_PIN);

    // CC11xx GPIO3
    GPIO_setAsOutputPin(CC11XX_GPIO3_PORT, CC11XX_GPIO3_PIN);
}

void cc11xx_wake_up()
{
    GPIO_setOutputLowOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);
    __delay_cycles(100);
    GPIO_setOutputHighOnPin(CC11XX_CSN_PORT, CC11XX_CSN_PIN);
}

//! \} End of cc11xx group
