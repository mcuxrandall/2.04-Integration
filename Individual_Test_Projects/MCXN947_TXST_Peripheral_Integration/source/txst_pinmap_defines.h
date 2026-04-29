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
 * @file    txst_pinmap_defines.h
 * @brief   A list of defines used to talk to the accompanying PCB.
 */

#ifndef TXST_PINMAP_DEFINES_H_
#define TXST_PINMAP_DEFINES_H_

// switches
#define SW0_head   GPIO4  // J4-1: P4_12
#define SW0_PIN    12U    // J4-1: P4_12
#define SW1_head   GPIO0  // J4-6: P0_14
#define SW1_PIN    14U    // J4-6: P0_14
#define SW2_head   GPIO0  // J4-8: P0_22
#define SW2_PIN    22U    // J4-8: P0_22

// LEDs
#define LED0_head  GPIO0  // J1-10: ARD_D4
#define LED0_PIN   30U    // J1-10: ARD_D4
#define LED1_head  GPIO4  // J4-5: P4_16
#define LED1_PIN   16U    // J4-5: P4_16
#define LED2_head  GPIO1  // J1-8: ARD_D3
#define LED2_PIN   23U    // J1-8: ARD_D3

// buttons
#define BTN0_head  GPIO4  // J4-9: P4_20
#define BTN0_PIN   20U    // J4-9: P4_20
#define BTN1_head  GPIO0  // J4-10: P0_15
#define BTN1_PIN   15U    // J4-10: P0_15

// FlexIO
#define FlexiPinOut  22U  // J8-27: P4_22
#define FlexiPinIn   18U  // J8-23: P4_18
#define FLEXIO_OUT_PIN 26U   // FLEXIO0_D26  -> P4_18
#define FLEXIO_IN_PIN  30U   // FLEXIO0_D30  -> P4_22

#define Pot0_head  ADC0  // J4-2: ADC0_A0
#define Pot0_PIN   0U  	 // J4-2: ADC0_A0



#define PWM_FREQ_HZ 20000U
#define CTIMER_BASE CTIMER0
#define CTIMER_INDEX 0U

// PWMA = PWM0, Ctimer0, match 0, header pin J2_8, output pin P0_24
#define PWMA_CTimer CTIMER0
#define PWMA_Match  kCTIMER_Match_0
#define AIN1_port GPIO2
#define AIN1_PIN  6U
#define AIN2_port GPIO2
#define AIN2_PIN  7U
#define STANDBY_port GPIO2
#define STANDBY_PIN  0U

// PWMA = PWM0, Ctimer0, match 0, header pin J2_8, output pin P0_24
#define PWMB_CTimer CTIMER0
#define PWMB_Match  kCTIMER_Match_2
#define BIN1_port GPIO2
#define BIN1_PIN  3U
#define BIN2_port GPIO1
#define BIN2_PIN  22U

#define ADC_BASE ADC0
#define ADC_CMD  1U
#define ADC_TRIG 1U
#define ADC_CH   0U   // ADC0_A0


#endif /* TXST_PINMAP_DEFINES_H_ */
