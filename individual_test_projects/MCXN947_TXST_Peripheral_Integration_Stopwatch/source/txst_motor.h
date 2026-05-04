/**
 * Texas State Senior Design, Fall 2025 - Spring 2026
 * Team 2.04: NXP Integration
 * 		Alexy Larios
 * 		Andrew Muegel
 * 		Matthew Coffelt
 * 		Stephen Lorenzen
 * Developed under an educational license
 *
 */

/*
 * @file    txst_motor.h
 * @brief   Programs the FRDM-MCXN947 to manipulate a PWM controlled motor driver (TB6612FNG).
 */


#ifndef TXST_MOTOR_H
#define TXST_MOTOR_H

#include "fsl_ctimer.h"
#include "fsl_gpio.h"
#include "txst_pinmap_defines.h"

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
};
#endif
