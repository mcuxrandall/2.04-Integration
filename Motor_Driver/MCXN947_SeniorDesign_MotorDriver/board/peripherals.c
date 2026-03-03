/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v1.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/*******************************************************************************
 * Included files
 ******************************************************************************/
#include "peripherals.h"
//#include "fsl_vref.h"
#include "fsl_lpadc.h"
#include "fsl_lpuart.h"
#include "fsl_clock.h"

/*******************************************************************************
 * BOARD_InitBootPeripherals function
 ******************************************************************************/

void BOARD_InitBootPeripherals(void)
{
	// LPADC default configs
	    lpadc_config_t adc1_config;
	    lpadc_conv_command_config_t adc1_commandConfig;
	    lpadc_conv_trigger_config_t adc1_triggerConfig;

	    LPADC_GetDefaultConfig(&adc1_config);
	    LPADC_Init(ADC1, &adc1_config);

	    LPADC_GetDefaultConvCommandConfig(&adc1_commandConfig);
	    // Example: set the channel to 0 (adjust if you use a different one)
	    adc1_commandConfig.channelNumber = 0U;
	    LPADC_SetConvCommandConfig(ADC1, 1U, &adc1_commandConfig);

	    LPADC_GetDefaultConvTriggerConfig(&adc1_triggerConfig);
	    adc1_triggerConfig.targetCommandId = 1U;
	    LPADC_SetConvTriggerConfig(ADC1, 0U, &adc1_triggerConfig);

	    // LPUART default config
	    lpuart_config_t lpuartConfig;
	    LPUART_GetDefaultConfig(&lpuartConfig);

	    // Use LPFlexComm clock getter for MCXN947
	    LPUART_Init(LPUART4, &lpuartConfig, CLOCK_GetLPFlexCommClkFreq(4));
}

