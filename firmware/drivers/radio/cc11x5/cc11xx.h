/*
 * cc11xx.h
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
 * \brief Functions of the CC1125/CC1175.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \defgroup cc11xx CC11XX
 * \ingroup drivers
 * \{
 */

#ifndef CC11XX_H_
#define CC11XX_H_

#include <stdint.h>
#include <config/config.h>
#include <drivers/driverlib/driverlib.h>

#include "cc11xx_pins.h"
#include "cc11xx_registers.h"

#define CC11XX_MODEL_LABEL              "CC1175"                /**< cc11xx model label */
#define CC11XX_SPI_CLK                  BEACON_RADIO_SPI_CLK    /**< SPI clock frequency in Hz. */

// CC1175 command strobes values (CC112x/CC1175 User's Guide Table 6)
#define CC11XX_SRES                     0x30        /**< Reset chip */
#define CC11XX_SFSTXON                  0x31        /**< Enable and calibrate frequency synthesizer (if SETTLING_CFG.FS_AUTOCAL = 1). If in RX and PKT_CFG2.CCA_MODE ≠ 0: Go to a wait state where only the synthesizer is running (for quick RX/TX turnaround). */
#define CC11XX_SXOFF                    0x32        /**< Enter XOFF state when CSn is de-asserted */
#define CC11XX_SCAL                     0x33        /**< Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without setting manual calibration mode (SETTLING_CFG.FS_AUTOCAL = 0) */
#define CC11XX_SRX                      0x34        /**< Enable RX. Perform calibration first if coming from IDLE and SETTLING_CFG.FS_AUTOCAL = 1 */
#define CC11XX_STX                      0x35        /**< In IDLE state: Enable TX. Perform calibration first if SETTLING_CFG.FS_AUTOCAL = 1. If in RX state and PKT_CFG2.CCA_MODE ≠ 0: Only go to TX if channel is clear */
#define CC11XX_SIDLE                    0x36        /**< Exit RX/TX, turn off frequency synthesizer and exit eWOR mode if applicable */
#define CC11XX_SAFC                     0x37        /**< Automatic Frequency Compensation */
#define CC11XX_SWOR                     0x38        /**< Start automatic RX polling sequence (eWOR) if WOR_CFGO.RC_PD = 0 */
#define CC11XX_SPWD                     0x39        /**< Enter SLEEP mode when CSn is de-asserted */
#define CC11XX_SFRX                     0x3A        /**< Flush the RX FIFO. Only issue SFRX in IDLE or RX_FIFO_ERR states */
#define CC11XX_SFTX                     0x3B        /**< Flush the TX FIFO. Only issue SFTX in IDLE or TX_FIFO_ERR states */
#define CC11XX_SWORRST                  0x3C        /**< Reset the eWOR timer to the Event1 value */
#define CC11XX_SNOP                     0x3D        /**< No operation. May be used to get access to the chip status byte */

#define CC11XX_DIRECT_MEMORY_ACCESS 0x3F

// FIFO access addresses (CC112x/CC1175 User's Guide Table 4)
#define CC11XX_SINGLE_TXFIFO            0x003F      /**< Single access to Transmit FIFO (0x3F + 0x00 = 0x003F) */
#define CC11XX_BURST_TXFIFO             0x007F      /**< Burst access to Transmit FIFO  (0x3F + 0x40 = 0x007F) */
#define CC11XX_SINGLE_RXFIFO            0x00BF      /**< Single access to Receive FIFO  (0x3F + 0x80 = 0x00BF) */
#define CC11XX_BURST_RXFIFO             0x00FF      /**< Burst access to Receive FIFO   (0x3F + 0xC0 = 0x00FF) */

// CC1175 access modes values (CC112x/CC1175 User's Guide Table 3)
#define CC11XX_BURST_ACCESS             0x40        /**< [R/W 1 A5 A4 A3 A2 A1 A0] = 0100 0000 = 0x40 */
#define CC11XX_SINGLE_ACCESS            0x00        /**< [R/W 0 A5 A4 A3 A2 A1 A0] = 0000 0000 = 0x00 */
#define CC11XX_READ_ACCESS              0x80        /**< [1 B/S A5 A4 A3 A2 A1 A0] = 1000 0000 = 0x80 */
#define CC11XX_WRITE_ACCESS             0x00        /**< [0 B/S A5 A4 A3 A2 A1 A0] = 0000 0000 = 0x00 */

// CC1175 Status Byte possible values (CC112x/CC1175 User's Guide Table 2)
#define CC11XX_STATUS_CHIP_RDYn_L       0x00    /**< Should always be low when using the SPI interface */
#define CC11XX_STATUS_CHIP_RDYn_H       0x80    /**< Stays high until power and crystal have stabilized */
#define CC11XX_STATE_IDLE               0x00    /**< Idle state */
#define CC11XX_STATE_RX                 0x10    /**< Receive mode */
#define CC11XX_STATE_TX                 0x20    /**< Transmit mode */
#define CC11XX_STATE_FSTXON             0x30    /**< Fast TX ready */
#define CC11XX_STATE_CALIBRATE          0x40    /**< Frequency synthesizer callibration is running */
#define CC11XX_STATE_SETTLING           0x50    /**< PLL is settling */
#define CC11XX_STATE_RX_FIFO_ERROR      0x60    /**< RX FIFO has over/underflowed. Read out any useful data, then flush the FIFO with an SFRX strobe */
#define CC11XX_STATE_TX_FIFO_ERROR      0x70    /**< TX FIFO has over/underflowed. Flush the FIFO with an SFTX strobe */

// Calibration const (CC112X, CC1175 Silicon Errata)
#define CC11XX_VCDAC_START_OFFSET       2
#define CC11XX_FS_VCO2_INDEX            0
#define CC11XX_FS_VCO4_INDEX            1
#define CC11XX_FS_CHP_INDEX             2

// GPIO signals (CC112x/CC1175 User's Guide Table 10)
#define CC11XX_GPIOX_ATRAN_STD_DIG_PAD  0x00    /**< Analog transfer enable: Standard digital pad. */
#define CC11XX_GPIOX_ATRAN_PAD_ANL_MODE 0x80    /**< Analog transfer enable: Pad in analog mode (digital GPIO input and output disabled). */
#define CC11XX_GPIOX_INV_DISABLE        0x00    /**< Invert output enable: Invert output disabled. */
#define CC11XX_GPIOX_INV_ENABLE         0x80    /**< Invert output enable: Invert output enable. */

//-- GPIOX_CFG -----------------------------
#define CC11XX_RXFIFO_THR               0x00    /**< Associated to the RX FIFO. Asserted when the RX FIFO is filled above FIFO_CFG.FIFO_THR. De-asserted when the RX FIFO is drained below (or is equal) to the same threshold. This signal is also available in the MODEM_STATUS1 register. */
#define CC11XX_RXFIFO_THR_PKT           0x01    /**< Associated to the RX FIFO. Asserted when the RX FIFO is filled above FIFO_CFG.FIFO_THR or the end of packet is reached. De-asserted when the RX FIFO is empty. */
#define CC11XX_TXFIFO_THR               0x02    /**< Associated to the TX FIFO. Asserted when the TX FIFO is filled above (or is equal to) (127 −FIFO_CFG.FIFO_THR). De-asserted when the TXFIFO is drained below the same threshold. This signal is also available in the MODEM_STATUS0 register. */
#define CC11XX_TXFIFO_THR_PKT           0x03    /**< Associated to the TX FIFO. Asserted when the TX FIFO is full. Deasserted when the TX FIFO is drained below (127 − FIFO_CFG.FIFO_THR). */
#define CC11XX_RXFIFO_OVERFLOW          0x04    /**< Asserted when the RX FIFO has overflowed. De-asserted when the RXFIFO is flushed (see Section 3.2.4). This signal is also available in the MODEM_STATUS1 register. */
#define CC11XX_TXFIFO_UNDERFLOW         0x05    /**< Asserted when the TX FIFO has underflowed. De-asserted when the TXFIFO is flushed (see Section 3.2.4). This signal is also available in the MODEM_STATUS0 register. */
#define CC11XX_PKT_SYNC_RXTX            0x06    /**< RX: Asserted when sync word has been received and de-asserted at the end of the packet. Will de-assert when the optional address and/or length check fails or the RX FIFO overflows/underflows. TX: Asserted when sync word has been sent, and de-asserted at the end of the packet. Will de-assert if the TX FIFO underflows/overflows. */
#define CC11XX_CRC_OK                   0x07    /**< Asserted simultaneously as PKT_CRC_OK. De-asserted when the first byte is read from the RX FIFO. */
#define CC11XX_SERIAL_CLK               0x08    /**< Serial clock (RX and TX mode). Synchronous to the data in synchronous serial mode. Data is set up on the falling edge in RX and is captured on the rising edge of the serial clock in TX. */
#define CC11XX_SERIAL_RX                0x09    /**< Serial data (RX mode). Used for both synchronous and transparent mode. Synchronous serial mode: Data is set up on the falling edge. Transparent mode: No timing recovery (outputs just the hard limited baseband signal). */
// 0x0A = Reserved (used for test)
#define CC11XX_PQT_REACHED              0x0B    /**< Preamble Quality Reached. Asserted when the quality of the preamble is above the programmed PQT value (see Section 6.8). This signal is also available in the MODEM_STATUS1 register. */
#define CC11XX_PQT_VALID                0x0C    /**< Preamble quality valid. Asserted when the PQT logic has received a sufficient number of symbols (see Section 6.8). This signal is also available in the MODEM_STATUS1 register. */
#define CC11XX_RSSI_VALID               0x0D    /**< RSSI calculation is valid. */
#define CC11XX_RSSI_UPDATE              0x0E    /**< Pins 3 and 2 = A pulse occurring each time the RSSI value is updated (see Figure 16). */
#define CC11XX_AGC_HOLD                 0x0E    /**< Pin 1 = AGC waits for gain settling (see Figure 16). */
#define CC11XX_AGC_UPDATE               0x0E    /**< Pin 0 = A pulse occurring each time the front end gain has been adjusted (see Figure 16). */
#define CC11XX_CCA_STATUS               0x0F    /**< Pins 3 and 1 = Current CCA status. */
#define CC11XX_TXONCCA_DONE             0x0F    /**< Pin 2 = A pulse occurring when a decision has been made as to whether the channel is busy or not. This signal must be used as an interrupt to the MCU. When this signal is asserted/de-asserted, TXONCCA_FAILED can be checked. */
#define CC11XX_TXONCCA_FAILED           0x0F    /**< Pin 0 = TX on CCA failed. This signal is also available in the MARC_STATUS0 register. */
#define CC11XX_CARRIER_SENSE_VALID      0x10    /**< CARRIER_SENSE is valid (see Figure 16). */
#define CC11XX_CARRIER_SENSE            0x11    /**< Carrier sense. High if RSSI level is above threshold (see section 6.9.1) (see Figure 16). */
#define CC11XX_DSSS_CLK                 0x12    /**< Pins 3 and 1 = DSSS clock (see Section 5.2.6 for more details). */
#define CC11XX_DSSS_DATA0               0x12    /**< Pin 2 = DSSS data0 (see Section 5.2.6 for more details). */
#define CC11XX_DSSS_DATA1               0x12    /**< Pin 0 = DSSS data1 (see Section 5.2.6 for more details). */
#define CC11XX_PKT_CRC_OK               0x13    /**< Asserted in RX when PKT_CFG1.CRC_CFG = 1 or 10 b and a good packet is received. This signal is always on if the radio is in TX or if the radio is in RX and PKT_CFG1.CRC_CFG = 0. The signal is de-asserted when RX mode is entered and PKT_CFG1.CRC_CFG ≠ 0. This signal is also available in the LQI_VAL register. */
#define CC11XX_MCU_WAKEUP               0x14    /**< MCU wake up signal. Read MARC_STATUS1.MARC_STATUS_OUT to find the cause of the wake up event (see Section 3.4.1.2 for more details). This signal is also available in the MARC_STATUS0 register. The signal is a pulse. */
#define CC11XX_SYNC_LOW0_HIGH1          0x15    /**< DualSync detect. Only valid when SYNC_CFG0.SYNC_MODE = 111b. When SYNC_EVENT is asserted this bit can be checked to see which sync word is found. This signal is also available in the DEM_STATUS register. */
// 0x16 = Reserved (used for test)
#define CC11XX_LNA_PA_REG_PD            0x17    /**< Common regulator control for PA and LNA. Indicates RF operation. */
#define CC11XX_LNA_PD                   0x18    /**< Control external LNA. */
#define CC11XX_PA_PD                    0x19    /**< Control external PA. */
#define CC11XX_RX0TX1_CFG               0x1A    /**< Indicates whether RF operation is in RX or TX (this signal is 0 in IDLE state). */
// 0x1B = Reserved (used for test)
#define CC11XX_IMAGE_FOUND              0x1C    /**< Image detected by image rejection calibration algorithm. */
#define CC11XX_CLKEN_CFM                0x1D    /**< Data clock for demodulator soft data (see Section 5.2.4 for more details). */
#define CC11XX_CFM_TX_DATA_CLK          0x1E    /**< Data clock for modulator soft data (see Section 5.2.4 for more details). */
// 0x1F - 0x20 = Reserved (used for test)
#define CC11XX_RSSI_STEP_FOUND          0x21    /**< RSSI step found during packet reception (after the assertion of SYNC_EVENT. The RSSI step is either 3 or 6 dB (configured through AGC_CFG3.RSSI_STEP_THR). This signal is also available in the DEM_STATUS register. */
#define CC11XX_RSSI_STEP_EVENT          0x22    /**< RSSI step detected. This signal is asserted if there is an RSSI step of 3 or 6 dB during sync search or during packet reception. The RSSI step is configured through AGC_CFG3.RSSI_STEP_THR). The signal is a pulse. */
#define CC11XX_LOCK                     0x23    /**< Pins 1 and 0 = Out of lock status signal. Indicates out of lock when the signal goes low. This signal is also available in the FSCAL_CTR register. */
#define CC11XX_ANTENNA_SELECT           0x24    /**< Antenna diversity control. Can be used to control external antenna switch. If differential signal is needed, two GPIOs can be used with one of them having IOCFGx.GPIOx_INV set to 1. */
#define CC11XX_MARC_2PIN_STATUS_1       0x25    /**< Partial MARC state status. These signals are also available in the MARCSTATE register. */
#define CC11XX_MARC_2PIN_STATUS_0       0x26    /**< See MARC_2PIN_STATUS_1. */
#define CC11XX_TXFIFO_OVERFLOW          0x27    /**< Pin 2 = Asserted when the TX FIFO has overflowed. De-asserted when the TXFIFO is flushed (see Section 3.2.4). This signal is also available in the MODEM_STATUS0 register. */
#define CC11XX_RXFIFO_UNDERFLOW         0x27    /**< Pin 0 = Asserted when the RX FIFO has underflowed. De-asserted when the RXFIFO is flushed (see Section 3.2.4). This signal is also available in the MODEM_STATUS1 register. */
#define CC11XX_MAGN_VALID               0x28    /**< Pin 3 = New CORDIC magnitude sample. */
#define CC11XX_CHFILT_VALID             0x28    /**< Pin 2 = New channel filter sample. */
#define CC11XX_RCC_CALL_VALID           0x28    /**< Pin 1 = RCOSC calibration has been performed at least once. */
#define CC11XX_CHFILT_STARTUP_VALID     0x28    /**< Pin 0 = Channel filter has settled. */
#define CC11XX_COLLISION_FOUND          0x29    /**< Pins 3 and 1 = Asserted if a sync word is found during packet reception (i.e. after SYNC_EVENT has been asserted) if MDMCFG1.COLLISION_DETECT_EN = 1. This signal is also available in the DEM_STATUS register. */
#define CC11XX_SYNC_EVENT               0x29    /**< Pin 2 = Sync word found (pulse). */
#define CC11XX_COLLISION_EVENT          0x29    /**< Pin 0 = Sync found during receive (pulse). */
#define CC11XX_PA_RAMP_UP               0x2A    /**< Asserted when ramping is started (for compliance testing). */
#define CC11XX_CRC_FAILED               0x2B    /**< Pin 3 = Packet CRC error. */
#define CC11XX_LENGTH_FAILED            0x2B    /**< Pin 2 = Packet length error. */
#define CC11XX_ADDR_FAILED              0x2B    /**< Pin 1 = Packet address error. */
#define CC11XX_UART_FRAMING_ERROR       0x2B    /**< Pin 0 = Packet UART framing error. */
#define CC11XX_AGC_STABLE_GAIN          0x2C    /**< AGC has settled to a gain. The AGC gain is reported stable whenever the current gain setting is equal to the previous gain setting. This condition is evaluated each time a new internal RSSI estimate is computed (see Figure 16). */
#define CC11XX_AGC_UPDATE_SIGNAL        0x2D    /**< A pulse occurring each time the front end gain has been adjusted (see Figure 16). */
#define CC11XX_ADC_CLOCK                0x2E    /**< Pins 3 and 1 = ADC clock. */
#define CC11XX_ADC_Q_DATA_SAMPLE        0x2E    /**< Pin 2 = ADC sample (Q data). */
#define CC11XX_ADC_I_DATA_SAMPLE        0x2E    /**< Pin 0 = ADC sample (I data). */
// 0x2F = Reserved (used for test)
#define CC11XX_HIGHZ                    0x30    /**< High impedance (tri-state). */
#define CC11XX_EXT_CLOCK                0x31    /**< External clock (divided crystal clock). The division factor is controlled through the ECG_CFG.EXT_CLOCK_FREQ register field. */
#define CC11XX_CHIP_RDYn                0x32    /**< Chip ready (XOSC is stable). */
#define CC11XX_HW0                      0x33    /**< HW to 0 (HW to 1 achieved with IOCFGx.GPIOx_INV = 1). */
// 0x34 - 0x35 = Reserved (used for test)
#define CC11XX_CLOCK_32K                0x36    /**< 32/40 kHz clock output from internal RC oscillator. */
#define CC11XX_WOR_EVENT0               0x37    /**< WOR EVENT0. */
#define CC11XX_WOR_EVENT1               0x38    /**< WOR EVENT1. */
#define CC11XX_WOR_EVENT2               0x39    /**< WOR EVENT2. */
// 0x3A = Reserved (used for test)
#define CC11XX_XOSC_STABLE              0x3B    /**< XOSC is stable (has finished settling). */
#define CC11XX_EXT_OSC_EN               0x3C    /**< External oscillator enable (used to control e.g. a TCXO). Note that this signal is only asserted is a TCXO is present. */
// 0x3D - 0x3F = Reserved (used for test)

/**
 * \brief cc11xx initialization
 * 
 * This function makes:
 *      -# Initialization of the Reset pin
 *      -# Chip reset (Manual) using cc11xx_ManualReset()
 *      -# Register configuration using cc11xx_RegConfig()
 *      .
 * 
 * \return Initialization status. It can be:
 * \parblock
 *      - STATUS_SUCCESS
 *      - STATUS_FAIL
 *      .
 * \endparblock
 */
uint8_t cc11xx_Init();

/**
 * \brief Configuration of the registers of the cc11xx
 * 
 * This function takes an array with the address of a register and its value,
 * and writes the data using cc11xx_WriteReg().
 * 
 * \return None.
 */
void cc11xx_RegConfig();

/**
 * \brief 
 * 
 * \param addr is the address of the register to be written.
 * \param pData is a pointer to the data to be written in the register.
 * \param len is the size of the data (*pData) to be written.
 * 
 * \return Chip status.
 */
uint8_t cc11xx_WriteReg(uint16_t addr, uint8_t *pData, uint8_t len);

/**
 * \brief 
 * 
 * \param addr is the address of the register to be written.
 * \param pData is a pointer to the data to be written in the register.
 * \param len is the size of the data (*pData) to be written.
 * 
 * \return Chip status.
 */
uint8_t cc11xx_ReadReg(uint16_t addr, uint8_t *pData, uint8_t len);

/**
 * \brief Sends a command strobe to the cc11xx.
 * 
 * \param cmd is the command strobe (See "CC112X/CC1175 User's Guide", table 6).
 * 
 * To get a list with all the possible commands, see: "CC112X/CC1175 User's Guide", table 6.
 * The table above is transcripted in \ref cmd_strobe CC11XX Command Strobes.
 * 
 * \return Chip status.
 */
uint8_t cc11xx_CmdStrobe(uint8_t cmd);

/**
 * \brief This function performs a read or write from/to a 8bit register
 *        address space. The function handles burst and single read/write
 *        as specfied in addrByte. Function assumes that chip is ready.
 * 
 * \param access_type Specifies if this is a read or write and if it's a
 *                    single or burst access. Bitmask made up of
 *                    RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                    RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * \param addr_byte address byte of register
 * \param pData data array
 * \param len length of array to be read (TX)/written (RX)
 * 
 * \return Chip status.
 */
uint8_t cc11xx_8BitRegAccess(uint8_t access_type, uint8_t addr_byte, uint8_t *pData, uint16_t len);

/**
 * \brief
 *
 * \param access_type
 * \param ext_addr
 * \param red_addr
 * \param pData
 * \param len
 * 
 * \return Chip status.
 */
uint8_t cc11xx_16BitRegAccess(uint8_t access_type, uint8_t ext_addr, uint8_t reg_addr, uint8_t *pData, uint8_t len);

/**
 * \brief Read/Write data using Burst/Single mode.
 * 
 * \param addr is the address of the register to be accessed.
 * \param pData is a pointer to the data to be written in the register (Write mode).
 *        Or a pointer to a variable to receive the register value.
 * \param len is the size of the data (pData).
 *
 * \return None.
 */
void cc11xx_ReadWriteBurstSingle(uint8_t addr, uint8_t *pData, uint16_t len);

/**
 * \brief Reset by using RESETn pin (active-low).
 * 
 * \return None.
 */
void cc11xx_ManualReset();

/**
 * \brief Chip calibration.
 * 
 * \see "CC112X, CC1175 Silicon Errata".
 * 
 * \return None.
 */
void cc11xx_ManualCalibration();

/**
 * \brief Writes data into the TX FIFO.
 * 
 * \param pData is a pointer to the data to be written.
 * \param len is the size of the data (*pData) to be written.
 * 
 * \return Chip status.
 */
uint8_t cc11xx_WriteTXFIFO(uint8_t *pData, uint8_t len);

/**
 * \brief Initialization of the MCU SPI.
 * 
 * Used interface: USCI_B0
 * 
 * \return Initialization status. It can be:
 *      - STATUS_SUCCESS
 *      - STATUS_FAIL
 *      .
 */
uint8_t cc11xx_SPI_Init();

/**
 * \brief Initialization of the MCU<=>cc11xx GPIOs.
 * 
 * \return None.
 */
void cc11xx_GPIO_Init();

/**
 * \brief Wakes up the radio if it is in sleep mode.
 * 
 * To wake up the radio in sleep mode, the CSn pin just need to be kept low.
 * 
 * \return None.
 */
void cc11xx_WakeUp();

#endif // CC11XX_H_

//! \} End of cc11xx group
