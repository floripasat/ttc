/*
 * rf6886.c
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
 * \file rf6886.c
 * 
 * \brief RF6889TR7 functions implementation
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

#include "../inc/rf6886.h"
#include "../inc/debug.h"

uint8_t rf6886_Init()
{
#if DEBUG_MODE == true
    debug_PrintMsg("RF6886 initialization... ");
#endif // DEBUG_MODE

    DAC12_A_initParam dac_params = {0};
    
    dac_params.submoduleSelect          = DAC12_A_SUBMODULE_0;
    dac_params.outputSelect             = DAC12_A_OUTPUT_1;
    dac_params.positiveReferenceVoltage = DAC12_A_VREF_AVCC;
    dac_params.outputVoltageMultiplier  = DAC12_A_VREFx1;
    dac_params.amplifierSetting         = DAC12_A_AMP_MEDIN_MEDOUT;
    dac_params.conversionTriggerSelect  = DAC12_A_TRIGGER_ENC;
    
    if (DAC12_A_init(DAC12_A_BASE, &dac_params) == STATUS_SUCCESS)
    {
        // ENE_3V3_PA_BEACON initialization
        GPIO_setAsOutputPin(ENE_3V3_PA_PORT, ENE_3V3_PA_PIN);
        GPIO_setOutputLowOnPin(ENE_3V3_PA_PORT, ENE_3V3_PA_PIN);
        
        // Calibrate output buffer for DAC12_A_0
        DAC12_A_calibrateOutput(DAC12_A_BASE, DAC12_A_SUBMODULE_0);
        
#if DEBUG_MODE == true
        debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE

        return STATUS_SUCCESS;
    }
    else
    {
#if DEBUG_MODE == true
        debug_PrintMsg("FAIL!\n");
#endif // DEBUG_MODE

        return STATUS_FAIL;
    }
}

void rf6886_Enable()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Enabling RF6886... ");
#endif // DEBUG_MODE

    GPIO_setOutputHighOnPin(ENE_3V3_PA_PORT, ENE_3V3_PA_PIN);

    DAC12_A_enableConversions(DAC12_A_BASE, DAC12_A_SUBMODULE_0);

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

void rf6886_Disable()
{
#if DEBUG_MODE == true
    debug_PrintMsg("Disabling RF6886... ");
#endif // DEBUG_MODE

    DAC12_A_disable(DAC12_A_BASE, DAC12_A_SUBMODULE_0);
    
    GPIO_setOutputLowOnPin(ENE_3V3_PA_PORT, ENE_3V3_PA_PIN);

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

void rf6886_SetVreg(float v_reg)
{
    if (v_reg > 3.3)
    {
#if DEBUG_MODE == true
        debug_PrintMsg("Error: RF6886 voltage gain must be lesser than 3,3V!\n");
#endif // DEBUG_MODE  
      
        v_reg = 3.3;
    }
    if (v_reg < 0)
    {
#if DEBUG_MODE == true
        debug_PrintMsg("Error: RF6886 voltage gain must be greater than 0V!\n");
#endif // DEBUG_MODE  
      
        v_reg = 0;
    }

#if DEBUG_MODE == true
    debug_PrintMsg("Setting RF6886 gain to ");
    
    float value = v_reg;
    int8_t digit_1 = -1;
    while(value >= 0)
    {
        value -= 1.00;
        digit_1++;
    }
    value += 1.0;
    
    int8_t digit_0 = -1;
    while(value >= 0)
    {
        value -= 0.10;
        digit_0++;
    }
    value += 0.1;
    
    debug_PrintDigit(digit_1);
    debug_PrintDigit(",");
    debug_PrintDigit(digit_0);
    debug_PrintMsg("V... ");
#endif // DEBUG_MODE

    // 12 bits = 0xFFF
    // V_REF = 0xFFF
    // v_reg = data
    // data  = gain*0xFFF/V_ref
    DAC12_A_setData(DAC12_A_BASE, DAC12_A_SUBMODULE_0, (uint16_t)(v_reg*0xFFF/V_REF));

#if DEBUG_MODE == true
    debug_PrintMsg("SUCCESS!\n");
#endif // DEBUG_MODE
}

//! \} End of rf6886 implementation group
