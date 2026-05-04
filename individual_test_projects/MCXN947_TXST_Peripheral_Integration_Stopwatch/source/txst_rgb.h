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
 * @file    txst_rgb.h
 * @brief   Programs the FRDM-MCXN947 to manipulate a common cathode RGB with PWM.
 */


#ifndef TXST_RGB_H
#define TXST_RGB_H

#include "fsl_pwm.h"

// PWM
#define RGBs_PWM_Type             PWM1
#define PWM_SRC_CLK_FREQ          CLOCK_GetFreq(kCLOCK_BusClk)
#define APP_DEFAULT_PWM_FREQUENCY (10000UL)

// initializes a module on PWM1
void Init_PWM_3Ch(pwm_submodule_t submodule);

// Initializes PWM1. Also calls Init_PWM_3Ch for modules 1 and 2.
status_t initialize_PWM1(void);

// sets the duty cycle for each of the LEDs and then sets the flag to update them
void output_RGB_val(uint32_t DutyCy_R, uint32_t DutyCy_G, uint32_t DutyCy_B);

#endif

