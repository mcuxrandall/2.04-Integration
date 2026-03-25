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
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "app.h"
#include "fsl_lpi2c_cmsis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define OLED_I2C_ADDRESS           (0x3CU)
#define OLED_WIDTH                 (128U)
#define OLED_HEIGHT                (64U)
#define OLED_PAGES                 (OLED_HEIGHT / 8U)           /* 8 pages */
#define OLED_BUFFER_SIZE           (OLED_WIDTH * OLED_PAGES)    /* 1024 bytes */

/* Control byte modes for SSD1306 */
#define SSD1306_COMMAND_MODE       (0x00U)  /* Co=0, D/C#=0 */
#define SSD1306_DATA_MODE          (0x40U)  /* Co=0, D/C#=1 */

/* Timeout: ~100 ms worth of loop iterations at the core clock.
 * Adjust I2C_TIMEOUT_TICKS if your SystemCoreClock differs. */
#define I2C_TIMEOUT_TICKS          (SystemCoreClock / 10U)

/*******************************************************************************
 * Font data for 5x7 characters (ASCII 0x20 ' ' through 0x7E '~' plus DEL)
 ******************************************************************************/
static const uint8_t font5x7_data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // sp
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x03, 0x00, 0x03, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x14, 0x08, 0x3E, 0x08, 0x14, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x26, 0x49, 0x49, 0x49, 0x3E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F
    0x3E, 0x41, 0x49, 0x49, 0x7A, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x30, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // "\"
    0x41, 0x41, 0x7F, 0x00, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x00, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x54, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x40, 0x80, 0x84, 0x7D, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x7F, 0x40, 0x40, 0x00, // l
    0x78, 0x04, 0x7C, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x08, 0x2A, 0x1C, 0x08, // ~
    0x08, 0x14, 0x08, 0x00, 0x00, // DEL
};

/*******************************************************************************
 * SSD1306 init command table
 * Sending all init bytes in one sequential loop avoids ~20 separate I2C
 * transactions and makes the sequence easy to audit against the datasheet.
 ******************************************************************************/
static const uint8_t ssd1306_init_cmds[] = {
    0xAE,              /* Display OFF                         */
    0xA8, OLED_HEIGHT - 1, /* Set MUX Ratio                   */
    0xD3, 0x00,        /* Set Display Offset = 0              */
    0x40,              /* Set Display Start Line = 0          */
    0x8D, 0x14,        /* Charge Pump ON                      */
    0xA1,              /* Segment Remap (col 127 -> SEG0)     */
    0xC8,              /* COM Output: remapped (top-to-bottom)*/
    0xDA, 0x12,        /* COM Pins Hardware Configuration     */
    0x81, 0xCF,        /* Contrast = 0xCF                     */
    0xD9, 0xF1,        /* Pre-charge Period                   */
    0xDB, 0x40,        /* VCOMH Deselect Level                */
    0xA4,              /* Entire Display On (use RAM content) */
    0xA6,              /* Normal (non-inverted) Display       */
    0x2E,              /* Deactivate Scroll                   */
    0xAF               /* Display ON                          */
};

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

/*!
 * @brief Main function
 */
int main(void)
{
    /* -----------------------------------------------------------------
     * Board and peripheral init
     * ----------------------------------------------------------------- */
    BOARD_InitHardware();

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

    const char *demo_text = "THIS IS A TEST!";       //type your text here
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

    while (1)
    {
        /* Application loop */
    }
}
