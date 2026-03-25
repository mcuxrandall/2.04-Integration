
#include "RGB.h"



// Set RGB PWM
	// PWM : PWM0 and PWM1 : The basic types
	// Submodule : kPWM_Module_0, 1, and 2 : Each submodule can have its own frequency
	// Channel : kPWM_PwmA, B, and X : Each channel can have its own duty cycle
	// Output : P0_2 = PWM1,A0 : Designating a pin's PWM, Channel, and Submodule
static void Init_PWM_3Ch(pwm_submodule_t submodule)
{
//    uint16_t deadTimeVal;
	pwm_signal_param_t pwmSignal[3];
	uint32_t pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
	uint32_t pwmFrequencyInHz = APP_DEFAULT_PWM_FREQUENCY;

	/* Set deadtime count, we set this to about 650ns */
//	deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

	// pwm_channels_t can be A, B, or X
	pwmSignal[0].pwmChannel = kPWM_PwmA;
	pwmSignal[1].pwmChannel = kPWM_PwmB;
	pwmSignal[2].pwmChannel = kPWM_PwmX;

	// The rest of the defaults are the same for each
	// However, the dutycycle field of PWMs B and X does not matter as we are
		// running in PWM A complementary mode

	pwmSignal[0].level            = kPWM_HighTrue;
	pwmSignal[0].dutyCyclePercent = 50; /* 1 percent dutycycle */
	pwmSignal[0].deadtimeValue    = 0; // 0 for LEDs
	pwmSignal[0].faultState       = kPWM_PwmFaultState0;
	pwmSignal[0].pwmchannelenable = true;

	pwmSignal[1].level      	  = kPWM_HighTrue;
	pwmSignal[1].dutyCyclePercent = 50;
	pwmSignal[1].deadtimeValue    = 0; // 0 for LEDs
	pwmSignal[1].faultState       = kPWM_PwmFaultState0;
	pwmSignal[1].pwmchannelenable = true;

	pwmSignal[2].level      	  = kPWM_HighTrue;
	pwmSignal[2].dutyCyclePercent = 50;
	pwmSignal[2].deadtimeValue    = 0; // 0 for LEDs
	pwmSignal[2].faultState       = kPWM_PwmFaultState0;
	pwmSignal[2].pwmchannelenable = true;

    // PWM_SetupPwm(PWM_Type *base, pwm_submodule_t subModule, const pwm_signal_param_t *chnlParams,
       // uint8_t numOfChnls, pwm_mode_t mode, uint32_t pwmFreq_Hz, uint32_t srcClock_Hz)
    PWM_SetupPwm(RGBs_PWM_Type, submodule, pwmSignal,
    		3, kPWM_EdgeAligned, pwmFrequencyInHz, pwmSourceClockInHz);

	// remove 'output disable' (a safety feature for motors) for submodules 1and 2
    RGBs_PWM_Type->SM[1].DISMAP[0] = 0;
	RGBs_PWM_Type->SM[1].DISMAP[1] = 0;
	RGBs_PWM_Type->SM[2].DISMAP[0] = 0;
	RGBs_PWM_Type->SM[2].DISMAP[1] = 0;
//	PWM0->SM[0].DISMAP[0] = 0;
//	PWM0->SM[0].DISMAP[1] = 0;


	// PWM_OutputEnable(PWM_Type *base, pwm_channels_t pwmChannel, pwm_submodule_t subModule);
	PWM_OutputEnable(RGBs_PWM_Type, kPWM_PwmA, submodule);
	PWM_OutputEnable(RGBs_PWM_Type, kPWM_PwmB, submodule);
	PWM_OutputEnable(RGBs_PWM_Type, kPWM_PwmX, submodule);
}

status_t initialize_PWM1(void) {
    pwm_config_t pwmConfig;
    PWM_GetDefaultConfig(&pwmConfig);


//    pwmConfig.enableDebugMode = false; 			// true pauses PWM
//    pwmConfig.reloadSelect = kPWM_LocalReload;  // local vs mod0 master
//    pwmConfig.clockSource = kPWM_BusClock; 		// IP bus clock
//    pwmConfig.prescale = kPWM_Prescale_Divide_1; // Clock source prescale
//    pwmConfig.initializationControl = kPWM_Initialize_LocalSync; // local vs mod0 master
    pwmConfig.forceTrigger = kPWM_Force_Local;
    	// for details, look up the type declaration for pwm_force_output_trigger_t
//    pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity; // every to every 160th
//    pwmConfig.reloadLogic = kPWM_ReloadImmediate; 	// load buffer immediately or cycle based
//    pwmConfig.pairOperation = kPWM_Independent; 	// ch A&B independent or shared


    /* Initialize submodules */
	if (PWM_Init(RGBs_PWM_Type, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
	{
		PRINTF("PWM initialization failed\n");
		return kStatus_Fail;
	}

	if (PWM_Init(RGBs_PWM_Type, kPWM_Module_1, &pwmConfig) == kStatus_Fail)
	{
		PRINTF("PWM initialization failed\n");
		return kStatus_Fail;
	}

	/* Initialize submodule 2 the same way as submodule 1 */
	if (PWM_Init(RGBs_PWM_Type, kPWM_Module_2, &pwmConfig) == kStatus_Fail)
	{
		PRINTF("PWM initialization failed\n");
		return kStatus_Fail;
	}

	// PWM1, channels A, B. and X
    // A is high, B is low, X is sync
	Init_PWM_3Ch(kPWM_Module_1);
	Init_PWM_3Ch(kPWM_Module_2);
	// Set the load okay bit for each PWM submodule to load registers from their buffer
	PWM_SetPwmLdok(RGBs_PWM_Type, kPWM_Control_Module_1, true);
	PWM_SetPwmLdok(RGBs_PWM_Type, kPWM_Control_Module_2, true);

	// Start the PWM generation from Submodule 1
	PWM_StartTimer(RGBs_PWM_Type, kPWM_Control_Module_1);
	PWM_StartTimer(RGBs_PWM_Type, kPWM_Control_Module_2);

	return kStatus_Success;
}
