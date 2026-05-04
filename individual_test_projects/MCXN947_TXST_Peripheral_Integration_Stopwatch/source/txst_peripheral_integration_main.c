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
 * @brief   Stopwatch with Start/Stop (BTN0) and Reset (BTN1) on SSD1306 OLED.
 *          Display format: MM:SS
 */

#include <stdio.h>
#include <string.h>

#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"

#include "txst_temp_p3t1755.h"
#include "fsl_p3t1755.h"
#include "txst_oled.h"

#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_device_registers.h"

#include "txst_pinmap_defines.h"
#include "txst_adc.h"
#include "txst_imu.h"
#include "txst_motor.h"
#include "txst_rgb.h"
#include "txst_sd_card.h"
#include "txst_temp_p3t1755.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOOP_DELAY_US  (1000000U)

#define COL0_X    (2U)
#define ROW1_Y    (8U)
#define ROW2_Y    (16U)
#define ROW3_Y    (24U)

#define LABEL_X   (4U)
#define LABEL_Y   ROW1_Y
#define VALUE_X   (20U)   /* centered for MM:SS */
#define VALUE_Y   ROW2_Y
#define UNIT_X    (4U)
#define UNIT_Y    ROW3_Y

/*******************************************************************************
 * Private helpers
 ******************************************************************************/

static void format_counter(char *buf, uint32_t seconds)
{
    uint32_t mins = seconds / 60U;
    uint32_t secs = seconds % 60U;
    snprintf(buf, 16, "%02lu:%02lu", (unsigned long)mins, (unsigned long)secs);
}

/*******************************************************************************
 * Main
 ******************************************************************************/

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("HELLO UART\r\n");
    PRINTF("Starting...\r\n");

    init_i3c_temp();
    PRINTF("Temp init done\r\n");

    if (!oled_init())
    {
        PRINTF("OLED init FAILED\r\n");
        while (1) {}
    }
    PRINTF("OLED init done\r\n");

    oled_clear();
    oled_refresh();
    oled_draw_text(COL0_X, ROW2_Y, "HELLO");
    oled_refresh();
    SDK_DelayAtLeastUs(1000000U, CLOCK_GetCoreSysClkFreq());

    PRINTF("Entering display loop...\r\n");

    if (initialize_PWM1() == kStatus_Fail) return 1;

    GPIO_PinWrite(STANDBY_port, STANDBY_PIN, 0U);

    uint8_t  btn0_state, btn1_state, sw0_state, sw1_state, sw2_state;
    uint32_t DutyCyVal_R = 0;
    uint32_t DutyCyVal_G = 0;
    uint32_t DutyCyVal_B = 0;
    uint32_t speed       = 0;
    bool     running     = false;
    uint32_t elapsed_sec = 0;
    char     counter_str[16];

    while (1)
    {
        /* ---- Buttons & switches ---------------------------------------- */
        btn0_state = GPIO_PinRead(BTN0_head, BTN0_PIN);
        btn1_state = GPIO_PinRead(BTN1_head, BTN1_PIN);
        sw0_state  = GPIO_PinRead(SW0_head,  SW0_PIN);
        sw1_state  = GPIO_PinRead(SW1_head,  SW1_PIN);
        sw2_state  = GPIO_PinRead(SW2_head,  SW2_PIN);

        /* BTN0 = Start/Stop (active low, 0 = pressed) */
        if (btn0_state == 0)
        {
            running = !running;
            while (GPIO_PinRead(BTN0_head, BTN0_PIN) == 0) {}
            SDK_DelayAtLeastUs(300000U, CLOCK_GetCoreSysClkFreq());
        }

        /* BTN1 = Reset */
        if (btn1_state == 0)
        {
            elapsed_sec = 0;
            running     = false;
            while (GPIO_PinRead(BTN1_head, BTN1_PIN) == 0) {}
            SDK_DelayAtLeastUs(300000U, CLOCK_GetCoreSysClkFreq());
        }

        /* ---- RGB PWM duty cycle ---------------------------------------- */
        if (sw0_state) DutyCyVal_R += 10;
        if (sw1_state) DutyCyVal_G += 10;
        if (sw2_state) DutyCyVal_B += 10;

        if (DutyCyVal_R > 100) DutyCyVal_R = 0;
        if (DutyCyVal_G > 100) DutyCyVal_G = 0;
        if (DutyCyVal_B > 100) DutyCyVal_B = 0;

        GPIO_PinWrite(LED0_head, LED0_PIN, sw0_state);
        GPIO_PinWrite(LED1_head, LED1_PIN, sw1_state);
        GPIO_PinWrite(LED2_head, LED2_PIN, sw2_state);
        output_RGB_val(DutyCyVal_R, DutyCyVal_G, DutyCyVal_B);

        /* ---- Motor ---------------------------------------------------- */
        CTIMER_UpdatePwmDutycycle(PWMA_CTimer, kCTIMER_Match_3, PWMA_Match, speed);
        CTIMER_UpdatePwmDutycycle(PWMB_CTimer, kCTIMER_Match_3, PWMB_Match, speed);
        GPIO_PinWrite(AIN1_port, AIN1_PIN,  sw0_state);
        GPIO_PinWrite(AIN2_port, AIN2_PIN, !sw0_state);
        GPIO_PinWrite(BIN1_port, BIN1_PIN,  sw0_state);
        GPIO_PinWrite(BIN2_port, BIN2_PIN, !sw0_state);
        GPIO_PinWrite(STANDBY_port, STANDBY_PIN, sw2_state);

        /* ---- OLED display --------------------------------------------- */
        format_counter(counter_str, elapsed_sec);

        oled_clear();
        oled_draw_text(LABEL_X, LABEL_Y,  "STOPWATCH");
        oled_draw_text(VALUE_X, VALUE_Y,  counter_str);
        oled_draw_text(UNIT_X,  UNIT_Y,   running ? "RUNNING" : "STOPPED");
        oled_refresh();

        PRINTF("Elapsed: %s  [%s]\r\n", counter_str, running ? "RUN" : "STOP");

        /* ---- Only count and delay if running -------------------------- */
        if (running)
        {
            SDK_DelayAtLeastUs(LOOP_DELAY_US, CLOCK_GetCoreSysClkFreq());
            elapsed_sec++;
        }
        else
        {
            SDK_DelayAtLeastUs(100000U, CLOCK_GetCoreSysClkFreq());
        }
    }
}
