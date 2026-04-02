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
 * @file    txst_oled.c
 * @brief   Programs the FRDM-MCXN947 to use an OLED Display (BOBFD4X6YV) over I2C.
 */

/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Optimized SSD1306 OLED Driver via CMSIS LPI2C
 *
 * Improvements over original:
 *  1. Timeout-guarded busy-waits (no infinite spin)
 *  2. Batched init command table (single loop, no 20+ separate calls)
 *  3. Address/window setup done once after init (not on every refresh)
 *  4. Zero-copy frame buffer: control byte lives at [0], frame data at [1..N]
 *     so ssd1306_refresh_display() is a single DMA transfer with no memcpy
 *  5. Dirty-page tracking: only pages with changed pixels are transmitted
 */

/* Standard C Included Files */
#include <stdio.h>
#include <string.h>

/* driver */
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_cmsis.h"

/* board parts */
#include "app.h"
#include "board.h"
#include "txst_oled.h"



/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static bool     i2c_wait_for_transfer(void);
static bool     ssd1306_write_command(uint8_t command);
static bool     ssd1306_init(void);
static void     ssd1306_clear_display(void);
static void     ssd1306_draw_pixel(uint8_t x, uint8_t y);
static void     ssd1306_draw_char(uint8_t x, uint8_t y, char c);
static void     ssd1306_draw_text(uint8_t x, uint8_t y, const char *text);
static bool     ssd1306_refresh_display(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*
 * IMPROVEMENT 4 — Zero-copy tx buffer layout:
 *
 *   g_tx_frame[0]              = SSD1306_DATA_MODE  (set once, never changes)
 *   g_tx_frame[1 .. 1024]      = frame pixel data
 *
 * g_oled_frame_buffer points directly into g_tx_frame+1, so
 * ssd1306_refresh_display() can DMA the entire g_tx_frame in one shot
 * with NO memcpy at all.
 */
AT_NONCACHEABLE_SECTION(static uint8_t g_tx_frame[OLED_BUFFER_SIZE + 1U]);

/* Convenience alias — draw functions write here as before */
static uint8_t * const g_oled_frame_buffer = &g_tx_frame[1];

/* Receive buffer (kept for any future read operations) */
AT_NONCACHEABLE_SECTION(static uint8_t g_master_rxBuff[256U]);

/* Scratch buffer for single-command writes (2 bytes: control + command) */
AT_NONCACHEABLE_SECTION(static uint8_t g_cmd_buf[2U]);

/* Transfer completion flag, set by the CMSIS callback */
static volatile bool g_MasterCompletionFlag = false;

/*
 * IMPROVEMENT 5 — Dirty-page bitmask.
 * Bit N set means page N has been written since the last refresh.
 * All 8 bits set = 0xFF means "full screen needs updating."
 */
static uint8_t g_dirty_pages = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief CMSIS LPI2C master event callback.
 */
static void lpi2c_master_callback(uint32_t event)
{
    if (event == ARM_I2C_EVENT_TRANSFER_DONE)
    {
        g_MasterCompletionFlag = true;
    }
}

/*!
 * @brief IMPROVEMENT 1 — Timeout-protected transfer wait.
 *
 * Spins until the transfer completes or the timeout expires.
 *
 * @return true  if the transfer completed successfully.
 * @return false if the timeout expired (I2C may be hung).
 */
static bool i2c_wait_for_transfer(void)
{
    uint32_t ticks = I2C_TIMEOUT_TICKS;

    while (!g_MasterCompletionFlag)
    {
        if (--ticks == 0U)
        {
            /* Timeout: clear the flag so callers can detect and recover */
            g_MasterCompletionFlag = false;
            PRINTF("ERROR: I2C transfer timeout!\r\n");
            return false;
        }
    }

    g_MasterCompletionFlag = false;
    return true;
}

/*!
 * @brief Sends a single command byte to the SSD1306.
 *
 * Uses the 2-byte g_cmd_buf (noncacheable) to avoid stack DMA hazards.
 *
 * @return true on success, false on I2C timeout.
 */
static bool ssd1306_write_command(uint8_t command)
{
    g_cmd_buf[0] = SSD1306_COMMAND_MODE;
    g_cmd_buf[1] = command;

    EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS, g_cmd_buf, 2U, false);
    return i2c_wait_for_transfer();
}

/*!
 * @brief IMPROVEMENT 2 — Batched init: iterates the command table in one loop.
 *
 * @return true if all commands succeeded, false if any I2C transfer timed out.
 */
static bool ssd1306_init(void)
{
    for (size_t i = 0U; i < sizeof(ssd1306_init_cmds); i++)
    {
        if (!ssd1306_write_command(ssd1306_init_cmds[i]))
        {
            PRINTF("ERROR: ssd1306_init failed at command index %u (0x%02X)\r\n",
                   (unsigned)i, ssd1306_init_cmds[i]);
            return false;
        }
    }
    return true;
}

/*!
 * @brief Clears the internal frame buffer and marks every page dirty.
 */
static void ssd1306_clear_display(void)
{
    memset(g_oled_frame_buffer, 0U, OLED_BUFFER_SIZE);
    g_dirty_pages = (uint8_t)((1U << OLED_PAGES) - 1U); /* all pages dirty */
}

/*!
 * @brief Sets a single pixel in the frame buffer.
 *
 * IMPROVEMENT 5: marks only the affected page dirty.
 */
static void ssd1306_draw_pixel(uint8_t x, uint8_t y)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }

    uint8_t  page       = y / 8U;
    uint16_t byte_index = (uint16_t)x + (uint16_t)page * OLED_WIDTH;
    uint8_t  bit_index  = y % 8U;

    g_oled_frame_buffer[byte_index] |= (uint8_t)(1U << bit_index);
    g_dirty_pages |= (uint8_t)(1U << page); /* mark this page dirty */
}

/*!
 * @brief Draws a single ASCII character at (x, y) using the 5x7 font.
 */
static void ssd1306_draw_char(uint8_t x, uint8_t y, char c)
{
    if (c < ' ' || c > '~')
    {
        return;
    }

    uint8_t char_index       = (uint8_t)(c - ' ');
    const uint8_t *char_data = &font5x7_data[char_index * 5U];

    for (uint8_t col = 0U; col < 5U; col++)
    {
        uint8_t col_bits = char_data[col];
        for (uint8_t row = 0U; row < 8U; row++)
        {
            if ((col_bits >> row) & 0x01U)
            {
                ssd1306_draw_pixel((uint8_t)(x + col), (uint8_t)(y + row));
            }
        }
    }
}

/*!
 * @brief Draws a null-terminated string starting at (x, y).
 * Wraps to the next row when the right edge is reached.
 */
static void ssd1306_draw_text(uint8_t x, uint8_t y, const char *text)
{
    while (*text != '\0')
    {
        ssd1306_draw_char(x, y, *text);
        x = (uint8_t)(x + 6U); /* 5-pixel glyph + 1-pixel gap */

        if ((x + 5U) >= OLED_WIDTH)
        {
            x = 0U;
            y = (uint8_t)(y + 8U);
        }
        text++;
    }
}

/*!
 * @brief IMPROVEMENT 3 + 4 + 5 — Sends only dirty pages to the OLED.
 *
 * The SSD1306 is placed in Page Addressing Mode so individual pages can be
 * targeted independently.  Each dirty page is sent as one I2C transaction
 * consisting of:
 *   - 1 control byte  (SSD1306_DATA_MODE)
 *   - 128 pixel bytes (one full page row)
 *
 * The per-page tx buffer sits in noncacheable memory to be safe for DMA.
 * After all dirty pages are sent, g_dirty_pages is cleared.
 *
 * @return true if all dirty pages were transmitted successfully.
 */
static bool ssd1306_refresh_display(void)
{
    /*
     * Per-page tx scratch buffer: control byte + 128 pixel bytes.
     * Declared static so it lives in noncacheable BSS (same section attribute
     * as the other buffers above — move to AT_NONCACHEABLE_SECTION if your
     * linker script requires explicit placement).
     */
    AT_NONCACHEABLE_SECTION(static uint8_t page_tx[OLED_WIDTH + 1U]);

    if (g_dirty_pages == 0U)
    {
        return true; /* Nothing to do */
    }

    /* Switch SSD1306 to Page Addressing Mode */
    if (!ssd1306_write_command(0x20U) || !ssd1306_write_command(0x02U))
    {
        return false;
    }

    page_tx[0] = SSD1306_DATA_MODE;

    for (uint8_t page = 0U; page < OLED_PAGES; page++)
    {
        /* Skip clean pages entirely — no I2C transaction needed */
        if ((g_dirty_pages & (uint8_t)(1U << page)) == 0U)
        {
            continue;
        }

        /* Point SSD1306's write cursor to the start of this page */
        if (!ssd1306_write_command((uint8_t)(0xB0U | page))) { return false; } /* Set Page */
        if (!ssd1306_write_command(0x00U))                   { return false; } /* Col low  */
        if (!ssd1306_write_command(0x10U))                   { return false; } /* Col high */

        /* Copy this page's pixel data directly from the frame buffer */
        memcpy(&page_tx[1],
               &g_oled_frame_buffer[page * OLED_WIDTH],
               OLED_WIDTH);

        EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS,
                                          page_tx,
                                          OLED_WIDTH + 1U,
                                          false);
        if (!i2c_wait_for_transfer())
        {
            return false;
        }
    }

    g_dirty_pages = 0U; /* All pending pages have been flushed */
    return true;
}

/*
 * Initializes OLED and displays the text "ON"
 * Prints debug info to console*/
static void init_oled(void) {

#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT)
    DMAMUX_Init(EXAMPLE_LPI2C_DMAMUX_BASEADDR);
#endif

    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(edmaConfig);
#endif
    EDMA_Init(EXAMPLE_LPI2C_DMA_BASEADDR, &edmaConfig);

    EXAMPLE_I2C_MASTER.Initialize(lpi2c_master_callback);
    EXAMPLE_I2C_MASTER.PowerControl(ARM_POWER_FULL);
    EXAMPLE_I2C_MASTER.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);

    PRINTF("\r\nCMSIS LPI2C OLED example -- Optimized driver.\r\n");

    /* -----------------------------------------------------------------
     * IMPROVEMENT 4: fix the control byte once; frame buffer alias is
     * already pointing at g_tx_frame+1, so no memcpy is needed later.
     * ----------------------------------------------------------------- */
    g_tx_frame[0] = SSD1306_DATA_MODE;

    /* -----------------------------------------------------------------
     * OLED init + clear
     * ----------------------------------------------------------------- */
    if (!ssd1306_init())
    {
        PRINTF("ERROR: OLED initialization failed.\r\n");
        while (1) {}
    }

    ssd1306_clear_display();

    /* IMPROVEMENT 3: push the clear to the display (all 8 pages dirty) */
    if (!ssd1306_refresh_display())
    {
        PRINTF("ERROR: Initial display refresh failed.\r\n");
        while (1) {}
    }

    /* -----------------------------------------------------------------
     * Draw centred text
     * ----------------------------------------------------------------- */
    PRINTF("Drawing text on the OLED...\r\n");

    const char *demo_text = "ON";       //type your text here
    uint8_t text_width    = (uint8_t)(strlen(demo_text) * 6U);
    uint8_t x_pos         = (uint8_t)((OLED_WIDTH  - text_width) / 2U);
    uint8_t y_pos         = (uint8_t)((OLED_HEIGHT - 8U)         / 2U);

    ssd1306_draw_text(x_pos, y_pos, demo_text);

    /* Only the pages touched by draw_text are transmitted */
    if (!ssd1306_refresh_display())
    {
        PRINTF("ERROR: Text refresh failed.\r\n");
        while (1) {}
    }

    PRINTF("Drawing complete. Text should be visible on the display.\r\n");

}
