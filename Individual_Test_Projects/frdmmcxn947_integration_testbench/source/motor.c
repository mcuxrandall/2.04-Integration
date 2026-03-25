#include "fsl_ctimer.h"
#include "fsl_gpio.h"
#include "motor.h"
#include "pinmap.h"

/* All CTIMER functions can be found in "fsl_ctimer.c" */

/* Initializes PWM timing */
void motor_pwm_init(void)
{
	ctimer_config_t config;
	CTIMER_GetDefaultConfig(&config);
	CTIMER_Init(CTIMER_BASE, &config);

	uint32_t srcClock = CLOCK_GetCTimerClkFreq(CTIMER_INDEX);

	CTIMER_SetupPwm(
			CTIMER_BASE,
			kCTIMER_Match_3,	// period
			PWMA_Match,	// duty
			100,				// duty cycle percentage
			PWM_FREQ_HZ,
			srcClock,
			false
	);

	CTIMER_SetupPwm(
			CTIMER_BASE,
			kCTIMER_Match_3,	// period
			PWMB_Match,	// duty
			100,				// duty cycle percentage
			PWM_FREQ_HZ,
			srcClock,
			false
	);
	CTIMER_StartTimer(CTIMER_BASE);
}




