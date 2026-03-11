#include "fsl_ctimer.h"
#include "fsl_gpio.h"
#include "motor.h"
#include "pinmap.h"

/* All CTIMER functions can be found in "fsl_ctimer.c" */

/* Initializes PWM timing */
static void pwm_init(void)
{
	ctimer_config_t config;
	CTIMER_GetDefaultConfig(&config);
	CTIMER_Init(CTIMER_BASE, &config);

	uint32_t srcClock = CLOCK_GetCTimerClkFreq(CTIMER_INDEX);

	CTIMER_SetupPwm(
			CTIMER_BASE,
			kCTIMER_Match_3,	// period
			kCTIMER_Match_0,	// duty
			100,				// duty cycle percentage
			PWM_FREQ_HZ,
			srcClock,
			false
	);

	CTIMER_StartTimer(CTIMER_BASE);
}

/* Initializes the two signal pins (AIN1/AIN2) to be output gpio */
void motor_init(void)
{
    gpio_pin_config_t out = { kGPIO_DigitalOutput, 0 };

    GPIO_PinInit(GPIO2, AIN1_PIN, &out);
    GPIO_PinInit(GPIO2, AIN2_PIN, &out);

    pwm_init();
}



