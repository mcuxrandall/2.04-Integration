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
#include "txst_oled.h"                /* oled_init/clear/draw_text/refresh()     */

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

    while (1)
    {
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
            SDK_DelayAtLeastUs(500000U, CLOCK_GetCoreSysClkFreq());
        }
    }
}
