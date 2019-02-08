/*
 * rf6886.c
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
 * \brief RF6889TR7 functions implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 23/09/2016
 * 
 * \addtogroup rf6886
 * \{
 */

#include <drivers/driverlib/driverlib.h>

#include "rf6886.h"
#include "rf6886_pinmap.h"
#include "rf6886_config.h"

uint8_t rf6886_init()
{
    DAC12_A_initParam dac_params = {0};
    
    dac_params.submoduleSelect          = RF6886_DAC_SUBMODULE;
    dac_params.outputSelect             = DAC12_A_OUTPUT_1;
    dac_params.positiveReferenceVoltage = DAC12_A_VREF_AVCC;
    dac_params.outputVoltageMultiplier  = DAC12_A_VREFx1;
    dac_params.amplifierSetting         = DAC12_A_AMP_MEDIN_MEDOUT;
    dac_params.conversionTriggerSelect  = DAC12_A_TRIGGER_ENC;
    
    if (DAC12_A_init(RF6886_DAC_BASE_ADDRESS, &dac_params) == STATUS_SUCCESS)
    {
        // ENE_3V3_PA_BEACON initialization
        GPIO_setAsOutputPin(RF6886_PORT, RF6886_PIN);
        GPIO_setOutputLowOnPin(RF6886_PORT, RF6886_PIN);
        
        // Calibrate output buffer
        DAC12_A_calibrateOutput(RF6886_DAC_BASE_ADDRESS, RF6886_DAC_SUBMODULE);

        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_FAIL;
    }
}

void rf6886_enable()
{
    GPIO_setOutputHighOnPin(RF6886_PORT, RF6886_PIN);

    DAC12_A_enableConversions(RF6886_DAC_BASE_ADDRESS, RF6886_DAC_SUBMODULE);
}

void rf6886_disable()
{
    DAC12_A_disable(RF6886_DAC_BASE_ADDRESS, RF6886_DAC_SUBMODULE);
    
    GPIO_setOutputLowOnPin(RF6886_PORT, RF6886_PIN);
}

void rf6886_SetVreg(float v_reg)
{
    if (v_reg > 3.3)
    {      
        v_reg = 3.3;
    }
    else if (v_reg < 0)
    {
        v_reg = 0;
    }

    // 12 bits = 0xFFF
    // V_REF = 0xFFF
    // v_reg = data
    // data  = gain*0xFFF/V_ref
    DAC12_A_setData(RF6886_DAC_BASE_ADDRESS, RF6886_DAC_SUBMODULE, (uint16_t)(v_reg*0xFFF/RF6886_V_REF));
}

//! \} End of rf6886 group
