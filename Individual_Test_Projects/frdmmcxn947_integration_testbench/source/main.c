
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "peripherals.h"
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "pinmap.h"		// has all pin defines

#include <stdio.h>
//#include "fsl_ctimer.h"
//#include "fsl_gpio.h"
#include "motor.h"
#include "adc.h"
#include "RGB.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

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



void SysTick_Handler(void)
{
    /* Toggle pin connected to LED */
    // GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
}



int main(void)
{

    // Board pins
    BOARD_InitHardware();	// includes GPIO pin setup
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();

	adc_init();			// handles all ADC logic
	motor_pwm_init();	// handles motor PWM logic
    if (initialize_PWM1() == kStatus_Fail) return 1; // handles RGB PWM

    // 1 turns the motor on, 0 turns it off
	GPIO_PinWrite(STANDBY_port, STANDBY_PIN, 0U);

    uint8_t btn0_state, btn1_state, sw0_state, sw1_state, sw2_state;
    uint32_t pwmVal_R = 0;
	uint32_t pwmVal_G = 0;
	uint32_t pwmVal_B = 0;
	uint8_t count, state;
	uint32_t raw, speed;
	while (1)
	{
		// Buttons are configured with pull-ups (active low). If pressed, GPIO read returns 0.
		btn0_state = GPIO_PinRead(BTN0_head, BTN0_PIN);
		btn1_state = GPIO_PinRead(BTN1_head, BTN1_PIN);
		sw0_state = GPIO_PinRead(SW0_head, SW0_PIN);
		sw1_state = GPIO_PinRead(SW1_head, SW1_PIN);
		sw2_state = GPIO_PinRead(SW2_head, SW2_PIN);

		//PWMs
		if (sw0_state) pwmVal_R += 10;
		if (sw1_state) pwmVal_G += 10;
		if (sw2_state) pwmVal_B += 10;

		if (pwmVal_R > 100) pwmVal_R = 0;
		if (pwmVal_G > 100) pwmVal_G = 0;
		if (pwmVal_B > 100) pwmVal_B = 0;

		if (btn0_state) {
			pwmVal_R = 0;
			pwmVal_G = 0;
			pwmVal_B = 0;
		}

		// Write LED = inverted button (so LED ON when button pressed)
		GPIO_PinWrite(LED0_head, LED0_PIN, sw0_state);
		GPIO_PinWrite(LED1_head, LED1_PIN, sw1_state);
		GPIO_PinWrite(LED2_head, LED2_PIN, sw2_state);

		// PWM_UpdatePwmDutycycle(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwmSignal,
								//  pwm_mode_t currPwmMode, uint8_t dutyCyclePercent)
        PWM_UpdatePwmDutycycle(RGBs_PWM_Type, kPWM_Module_1, kPWM_PwmA, kPWM_EdgeAligned, pwmVal_R);
        PWM_UpdatePwmDutycycle(RGBs_PWM_Type, kPWM_Module_1, kPWM_PwmB, kPWM_EdgeAligned, pwmVal_G);
        PWM_UpdatePwmDutycycle(RGBs_PWM_Type, kPWM_Module_2, kPWM_PwmA, kPWM_EdgeAligned, pwmVal_B);

        /* Set the load okay bit for all submodules to load registers from their buffer */
        PWM_SetPwmLdok(RGBs_PWM_Type, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);

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


		// GPIO_PinWrite(LED1_head, LED1_PIN, 1U);
		SDK_DelayAtLeastUs(300000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

	}
}
