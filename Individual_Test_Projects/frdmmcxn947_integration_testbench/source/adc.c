#include "fsl_lpadc.h"
#include "adc.h"
#include "pinmap.h"
#include "fsl_spc.h"

/* All LPADC functions can be found in "fsl_lpadc.c" */
/* All CLOCK functions can be found in "fsl_clock.c" */

/* Initializes ADC configuration */
void adc_init(void)
{
	/* Reset ADC0 */
	RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);

    /* Configure ADC clock */
    CLOCK_EnableClock(kCLOCK_FroHf);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);
    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);

    /* LPADC base config */
    lpadc_config_t config;
    LPADC_GetDefaultConfig(&config);
    config.referenceVoltageSource  = kLPADC_ReferenceVoltageAlt1;
    config.enableAnalogPreliminary = true;
    config.powerUpDelay            = 0xFFU;
    LPADC_Init(ADC0, &config);

    LPADC_DoAutoCalibration(ADC0);

    /* Command config */
    lpadc_conv_command_config_t cmd;
    LPADC_GetDefaultConvCommandConfig(&cmd);
    cmd.channelNumber     = 0U;   // ADC0_A0
    cmd.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    LPADC_SetConvCommandConfig(ADC0, ADC_CMD, &cmd);

    /* Trigger config */
    lpadc_conv_trigger_config_t trig;
    LPADC_GetDefaultConvTriggerConfig(&trig);
    trig.targetCommandId       = 1U;
    trig.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC0, ADC_TRIG, &trig);
}

/* Gathers raw ADC data and shifts it 3 bits to get it 0-4095 */
uint16_t adc_read_pot(void)
{
    lpadc_conv_result_t result;

    LPADC_DoSoftwareTrigger(ADC0, 1U << ADC_TRIG);

    while (!LPADC_GetConvResult(ADC0, &result, 0)) {
    }

    return (result.convValue >> 3);   // already 0–4095
}






