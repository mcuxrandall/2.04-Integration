/**
 * Texas State Senior Design, Fall 2025 - Spring 2026
 * Team 2.04: NXP Integration
 * 		Alexy Larios
 * 		Andrew Muegel
 * 		Matthew Coffelt
 * 		Stephen Lorenzen
 * Developed under an educational license
 * This project programs the FRDM-MCXN947 to use the printed circuit board designed by Texas State Senior Design Team 2.03: Diamond Dogs (Fall 2025 - Spring 2026).
 * This project is intended as an educational tool to accompany the laboratory section of Texas State EE3420 - Microprocessors.
 *
 */

/*
 * @file    txst_peripheral_integration_main.c
 * @brief   Runs a test bench to show the accompanying PCB's full functionality.
 */

#include <stdio.h>
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "fsl_device_registers.h"
#include "app.h"
#include "board.h"

#include "txst_pinmap_defines.h"		// gathers the defines for readability
#include "txst_adc.h"
#include "txst_imu.h"
#include "txst_motor.h"
#include "txst_oled.h"
#include "txst_rgb.h"
#include "txst_sd_card.h"
//#include "txst_temp_lm75b.h"
#include "txst_temp_p3t1755.h"


//void SysTick_Handler(void)
//{
//    /* Toggle pin connected to LED */
//    // GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
//    // If you run the LED Blinky demo with no systick, you get stuck in an infinite loop.
//}

/*
 * TO ADD I3C TEMP READ FUNCTION, CALL the read_i3c_temp() function
 */


int main(void) {


    BOARD_InitHardware();	// includes GPIO pin setup
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

	adc_init();			// handles all ADC logic
	motor_pwm_init();	// handles motor PWM logic
    if (initialize_PWM1() == kStatus_Fail) return 1; // handles RGB P

    // 1 turns the motor on, 0 turns it off
	GPIO_PinWrite(STANDBY_port, STANDBY_PIN, 0U);

	uint8_t btn0_state, btn1_state, sw0_state, sw1_state, sw2_state;
	uint32_t DutyCyVal_R = 0;
	uint32_t DutyCyVal_G = 0;
	uint32_t DutyCyVal_B = 0;
	uint8_t count, state;
	uint32_t raw, speed;
    while(1) {
    	// Buttons are configured with pull-ups (active low). If pressed, GPIO read returns 0.
		btn0_state = GPIO_PinRead(BTN0_head, BTN0_PIN);
		btn1_state = GPIO_PinRead(BTN1_head, BTN1_PIN);
		sw0_state = GPIO_PinRead(SW0_head, SW0_PIN);
		sw1_state = GPIO_PinRead(SW1_head, SW1_PIN);
		sw2_state = GPIO_PinRead(SW2_head, SW2_PIN);

		//PWMs
		if (sw0_state) DutyCyVal_R += 10;
		if (sw1_state) DutyCyVal_G += 10;
		if (sw2_state) DutyCyVal_B += 10;

		if (DutyCyVal_R > 100) DutyCyVal_R = 0;
		if (DutyCyVal_G > 100) DutyCyVal_G = 0;
		if (DutyCyVal_B > 100) DutyCyVal_B = 0;

		if (btn0_state) {
			DutyCyVal_R = 0;
			DutyCyVal_G = 0;
			DutyCyVal_B = 0;
		}

		// Write LED = inverted button (so LED ON when button pressed)
		GPIO_PinWrite(LED0_head, LED0_PIN, sw0_state);
		GPIO_PinWrite(LED1_head, LED1_PIN, sw1_state);
		GPIO_PinWrite(LED2_head, LED2_PIN, sw2_state);

		// Set RGB colors
		output_RGB_val(DutyCyVal_R, DutyCyVal_G, DutyCyVal_B);

		// Motor
		raw = adc_read_pot();					// raw ADC data
		speed = raw * 100 / 4095;				// raw data turned into percentage (CTIMER takes percentage 0-100)
		/* speed clamping to not exceed 4095 (100%) */
		if(speed > 100){
			speed = 100;
		}

		/* Prints data to terminal */
		PRINTF("\rSTDBY(sw2): %1u | Dir(Sw0): %1u | RAW: %5u | SPEED: %3u%%   ",
				sw2_state, sw0_state, raw, speed );


		/* This is what sends the PWM through the PWM pin */
		CTIMER_UpdatePwmDutycycle(PWMA_CTimer, kCTIMER_Match_3, PWMA_Match, speed); // your specific CTIMER, period, duty, speed percentage
		CTIMER_UpdatePwmDutycycle(PWMB_CTimer, kCTIMER_Match_3, PWMB_Match, speed); // your specific CTIMER, period, duty, speed percentage

		/* motor direction? */
		GPIO_PinWrite(AIN1_port, AIN1_PIN, sw0_state);
		GPIO_PinWrite(AIN2_port, AIN2_PIN, !sw0_state);
		GPIO_PinWrite(BIN1_port, BIN1_PIN, sw0_state);
		GPIO_PinWrite(BIN2_port, BIN2_PIN, !sw0_state);
		GPIO_PinWrite(STANDBY_port, STANDBY_PIN, sw2_state);


		// delay to avoid throttling
		SDK_DelayAtLeastUs(300000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    }
    return 0;
}
