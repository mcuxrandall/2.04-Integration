#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "motor.h"
#include "adc.h"
#include "fsl_gpio.h"
#include "pinmap.h"		// has all pin defines


int main(void) {

	/* Board initializations from pin_mux */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();

	adc_init();		// handles all ADC logic
	motor_init();	// handles all PWM logic

	while(1){
		uint32_t sw = GPIO_PinRead(GPIO0, SW1_PIN);		// read sw state
		uint32_t raw = adc_read_pot();					// raw ADC data
		uint32_t speed = raw * 100 / 4095;				// raw data turned into percentage (CTIMER takes percentage 0-100)

		 /* Prints data to terminal */
		PRINTF("\rSW: %1u | RAW: %5u | SPEED: %3u%%   ",
		       sw,
		       raw,
		       speed
		);

		/* speed clamping to not exceed 4095 (100%) */
		if(speed > 100){
			speed = 100;
		}

		/* This is what sends the PWM through the PWM pin */
		CTIMER_UpdatePwmDutycycle(CTIMER0, kCTIMER_Match_3, kCTIMER_Match_0, speed); // your specific CTIMER, period, duty, speed percentage

		/* if sw = 1 */
		if(sw){
			GPIO_PinWrite(GPIO2, AIN1_PIN, 1);
			GPIO_PinWrite(GPIO2, AIN2_PIN, 0);
		}

		/* if sw = 0 */
		if(!sw){
			GPIO_PinWrite(GPIO2, AIN1_PIN, 0);
			GPIO_PinWrite(GPIO2, AIN2_PIN, 1);
		}
	}
}
