/**
 * Texas State Senior Design, Fall 2025 - Spring 2026
 * Team 2.04: NXP Integration
 *      Alexy Larios
 *      Andrew Muegel
 *      Matthew Coffelt
 *      Stephen Lorenzen
 * Developed under an educational license
 */

/*
 * @file    main.c
 * @brief   Reads the P3T1755DP internal temperature sensor and displays
 *          the live reading on the SSD1306 OLED.
 *
 *          Wiring:
 *            - Temp sensor : I3C  (handled by txst_temp_p3t1755.c)
 *            - OLED display: LPI2C via DMA (handled by oled.c)
 *          Both buses are independent so there is no address conflict.
 */

#include <stdio.h>
#include <string.h>

#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"

#include "txst_temp_p3t1755.h"   /* init_i3c_temp(), p3t1755Handle          */
#include "fsl_p3t1755.h"         /* P3T1755_ReadTemperature()               */
#include "txst_oled.h"     /* oled_init/clear/draw_text/refresh()     */
//#include "txst_p3t1755.c"
//#include "fsl_p3t1755.h"
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* How long to wait between display updates (microseconds).
 * 1 000 000 us = 1 second — plenty of time for a temp sensor.
 * Lower this if you want faster updates; the OLED can handle it easily. */


/* Pixel position for the label and the reading.
 * The display is 128x64.  Each text row is 8 px tall.
 * We use two rows: a static label on row 2, the value on row 4. */
#define LABEL_X   (22U)   /* centres "TEMPERATURE" (11 chars * 6 px = 66 px) */
#define LABEL_Y   (16U)   /* row 2                                            */
#define VALUE_X   (20U)   /* centres "  -xx.xx C  " roughly                  */
#define VALUE_Y   (32U)   /* row 4                                            */
#define UNIT_X    (20U)
#define UNIT_Y    (44U)   /* row 5 — shows "Fahrenheit" equivalent           */

/*******************************************************************************
 * Private helpers
 ******************************************************************************/

/*!
 * @brief Formats a double temperature into a display string.
 *
 * Produces strings like:
 *   " 23.75 C"   (positive)
 *   "-10.50 C"   (negative)
 *
 * We avoid printf("%f") in the loop to keep stack usage minimal on the MCU.
 * snprintf is fine here — it is already pulled in by fsl_debug_console.
 *
 * @param buf     Destination buffer (at least 16 bytes).
 * @param temp_c  Temperature in degrees Celsius.
 */
static void format_temperature(char *buf, double temp_c)
{
    /* Split into whole degrees and hundredths, handling the negative case. */
    int whole;
    int frac;

    if (temp_c < 0.0)
    {
        /* e.g. -10.5 -> whole = -10, frac = 50 */
        whole = (int)temp_c;
        frac  = (int)(-(temp_c - (double)whole) * 100.0 + 0.5);
    }
    else
    {
        whole = (int)temp_c;
        frac  = (int)((temp_c - (double)whole) * 100.0 + 0.5);
    }

    /* Handle rounding carry (e.g. frac == 100) */
    if (frac >= 100)
    {
        frac -= 100;
        whole += (temp_c < 0.0) ? -1 : 1;
    }

    snprintf(buf, 16, "%4d.%02d C", whole, frac);
}

/*!
 * @brief Converts Celsius to Fahrenheit and formats it.
 *
 * @param buf     Destination buffer (at least 16 bytes).
 * @param temp_c  Temperature in degrees Celsius.
 */
static void format_fahrenheit(char *buf, double temp_c)
{
    double temp_f = (temp_c * 9.0 / 5.0) + 32.0;

    int whole = (int)temp_f;
    int frac;

    if (temp_f < 0.0)
    {
        frac = (int)(-(temp_f - (double)whole) * 100.0 + 0.5);
    }
    else
    {
        frac = (int)((temp_f - (double)whole) * 100.0 + 0.5);
    }

    if (frac >= 100)
    {
        frac -= 100;
        whole += (temp_f < 0.0) ? -1 : 1;
    }

    snprintf(buf, 16, "%4d.%02d F", whole, frac);
}

/*******************************************************************************
 * Main
 ******************************************************************************/

int main(void)
{
    /* ------------------------------------------------------------------
     * 1. Board init
     * ------------------------------------------------------------------ */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    //p3t1755_handle_t p3t1755Handle;

    PRINTF("HELLO UART\r\n");

    /* ------------------------------------------------------------------
     * DEBUG INIT SEQUENCE (added)
     * ------------------------------------------------------------------ */
    PRINTF("Starting...\r\n");

    init_i3c_temp();
    PRINTF("Temp init skipped\r\n");

    if (!oled_init())
    {
        PRINTF("OLED init FAILED\r\n");
        while (1) {}
    }
    PRINTF("OLED init done\r\n");

    oled_clear();
    oled_draw_text(0, 0, "HELLO");
    oled_refresh();
    PRINTF("Refresh done\r\n");

    /* Optional: small delay so you can actually SEE "HELLO" */
    SDK_DelayAtLeastUs(1000000U, CLOCK_GetCoreSysClkFreq());

    /* ------------------------------------------------------------------
     * Draw static label once
     * ------------------------------------------------------------------ */
    oled_clear();
    oled_draw_text(LABEL_X, LABEL_Y, "TEMPERATURE");
    oled_refresh();

    PRINTF("Entering display loop...\r\n");

    /* ------------------------------------------------------------------
     * Main loop
     * ------------------------------------------------------------------ */
    char    celsius_str[16];
    char    fahrenheit_str[16];
    double  temperature = 0.0;

	//adc_init();			// handles all ADC logic
	//motor_pwm_init();	// handles motor PWM logic
    if (initialize_PWM1() == kStatus_Fail) return 1; // handles RGB P

    // 1 turns the motor on, 0 turns it off
	GPIO_PinWrite(STANDBY_port, STANDBY_PIN, 0U);

	uint8_t btn0_state, btn1_state, sw0_state, sw1_state, sw2_state;
	uint32_t DutyCyVal_R = 0;
	uint32_t DutyCyVal_G = 0;
	uint32_t DutyCyVal_B = 0;
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
		if (sw0_state) DutyCyVal_R += 10;
		if (sw1_state) DutyCyVal_G += 10;
		if (sw2_state) DutyCyVal_B += 10;

		if (DutyCyVal_R > 100) DutyCyVal_R = 0;
		if (DutyCyVal_G > 100) DutyCyVal_G = 0;
		if (DutyCyVal_B > 100) DutyCyVal_B = 0;

		/*
		if (btn0_state) {
			DutyCyVal_R = 0;
			DutyCyVal_G = 0;
			DutyCyVal_B = 0;
		}*/

		// Write LED = inverted button (so LED ON when button pressed)
		GPIO_PinWrite(LED0_head, LED0_PIN, sw0_state);
		GPIO_PinWrite(LED1_head, LED1_PIN, sw1_state);
		GPIO_PinWrite(LED2_head, LED2_PIN, sw2_state);

		// Set RGB colors
		output_RGB_val(DutyCyVal_R, DutyCyVal_G, DutyCyVal_B);

		// Motor
		//raw = adc_read_pot();					// raw ADC data
		//speed = raw * 100 / 4095;				// raw data turned into percentage (CTIMER takes percentage 0-100)
		/* speed clamping to not exceed 4095 (100%) */
		//if(speed > 100){
			//speed = 100;
		//}

		/* Prints data to terminal */
		//PRINTF("\rSTDBY(sw2): %1u | Dir(Sw0): %1u | RAW: %5u | SPEED: %3u%%   ",
		//		sw2_state, sw0_state, raw, speed );


		/* This is what sends the PWM through the PWM pin */
		CTIMER_UpdatePwmDutycycle(PWMA_CTimer, kCTIMER_Match_3, PWMA_Match, speed); // your specific CTIMER, period, duty, speed percentage
		CTIMER_UpdatePwmDutycycle(PWMB_CTimer, kCTIMER_Match_3, PWMB_Match, speed); // your specific CTIMER, period, duty, speed percentage

		/* motor direction? */
		GPIO_PinWrite(AIN1_port, AIN1_PIN, sw0_state);
		GPIO_PinWrite(AIN2_port, AIN2_PIN, !sw0_state);
		GPIO_PinWrite(BIN1_port, BIN1_PIN, sw0_state);
		GPIO_PinWrite(BIN2_port, BIN2_PIN, !sw0_state);
		GPIO_PinWrite(STANDBY_port, STANDBY_PIN, sw2_state);

        status_t result = P3T1755_ReadTemperature(&p3t1755Handle, &temperature);

        if (result != kStatus_Success)
        {
            PRINTF("WARNING: Temperature read failed (status %d)\r\n", (int)result);
            oled_draw_text(VALUE_X, VALUE_Y, "READ ERR");
            oled_refresh();
        }
        else
        {
            format_temperature(celsius_str, temperature);
            format_fahrenheit(fahrenheit_str, temperature);

            PRINTF("Temperature: %s  /  %s\r\n", celsius_str, fahrenheit_str);

            /* Draw updated reading */
            oled_clear();
            oled_draw_text(LABEL_X, LABEL_Y, "TEMPERATURE");
            oled_draw_text(VALUE_X, VALUE_Y, celsius_str);
            oled_draw_text(UNIT_X,  UNIT_Y,  fahrenheit_str);
            oled_refresh();

            /* Show for 5 seconds */
            SDK_DelayAtLeastUs(5000000U, CLOCK_GetCoreSysClkFreq());

            /* Clear screen */
            oled_clear();
            oled_refresh();

            /* Brief pause before next reading */
            //SDK_DelayAtLeastUs(500000U, CLOCK_GetCoreSysClkFreq());
        }
    }
}
