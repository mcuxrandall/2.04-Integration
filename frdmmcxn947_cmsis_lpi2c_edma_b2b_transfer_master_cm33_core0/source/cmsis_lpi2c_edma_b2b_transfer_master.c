/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#define OLED_WIDTH                 (128)
#define OLED_HEIGHT                (64)
#define OLED_BUFFER_SIZE           ((OLED_WIDTH * OLED_HEIGHT) / 8)

/* Command and data modes for SSD1306 */
#define SSD1306_COMMAND_MODE       (0x00U)
#define SSD1306_DATA_MODE          (0x40U)

/* Define the font scaling factor. Change this value to adjust the text size. */
#define FONT_SCALE_FACTOR          3

/* Font data for 5x7 characters. */
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
    0x08, 0x14, 0x3E, 0x01, 0x01, // g
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
 * Prototypes
 ******************************************************************************/

static void lpi2c_master_callback(uint32_t event);
static void ssd1306_write_command(uint8_t command);
static void ssd1306_write_data(const uint8_t *data, size_t size);
static void ssd1306_init(void);
static void ssd1306_clear_display(void);
static void ssd1306_draw_pixel(uint8_t x, uint8_t y);
static void ssd1306_draw_char(uint8_t x, uint8_t y, char c);
static void ssd1306_draw_text(uint8_t x, uint8_t y, const char* text);
static void ssd1306_refresh_display(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION(uint8_t g_master_txBuff[256]);
AT_NONCACHEABLE_SECTION(uint8_t g_master_rxBuff[256]);
AT_NONCACHEABLE_SECTION(uint8_t g_oled_frame_buffer[OLED_BUFFER_SIZE]);
volatile bool g_MasterCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_master_callback(uint32_t event)
{
    switch (event)
    {
        /* The master has sent a stop transition on the bus */
        case ARM_I2C_EVENT_TRANSFER_DONE:
            g_MasterCompletionFlag = true;
            break;

        default:
            break;
    }
}

/*!
 * @brief Helper function to write a single command to the SSD1306.
 */
static void ssd1306_write_command(uint8_t command)
{
    uint8_t i2c_data[2];
    i2c_data[0] = SSD1306_COMMAND_MODE; /* Co bit (0), D/C# bit (0) */
    i2c_data[1] = command;

    EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS, i2c_data, 2, false);
    while (!g_MasterCompletionFlag) {}
    g_MasterCompletionFlag = false;
}

/*!
 * @brief Helper function to write data to the SSD1306.
 */
static void ssd1306_write_data(const uint8_t *data, size_t size)
{
    /* The first byte is the control byte (Co = 0, D/C# = 1) */
    EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS, data, size, false);
    while (!g_MasterCompletionFlag) {}
    g_MasterCompletionFlag = false;
}

/*!
 * @brief Initializes the SSD1306 OLED display.
 */
static void ssd1306_init(void)
{
    /*
     * The following is a basic initialization sequence for the SSD1306.
     * Refer to the SSD1306 datasheet for more details on each command.
     */
    ssd1306_write_command(0xAE); /* Turn off display */
    ssd1306_write_command(0xA8); /* Set MUX Ratio */
    ssd1306_write_command(OLED_HEIGHT - 1);
    ssd1306_write_command(0xD3); /* Set Display Offset */
    ssd1306_write_command(0x00);
    ssd1306_write_command(0x40); /* Set Start Line */
    ssd1306_write_command(0x8D); /* Charge Pump Setting */
    ssd1306_write_command(0x14);
    ssd1306_write_command(0xA1); /* Segment Remap (for correct orientation) */
    ssd1306_write_command(0xC8); /* COM Output Scan Direction */
    ssd1306_write_command(0xDA); /* COM Pins Hardware Configuration */
    ssd1306_write_command(0x12);
    ssd1306_write_command(0x81); /* Set Contrast Control */
    ssd1306_write_command(0xCF);
    ssd1306_write_command(0xD9); /* Set Pre-charge Period */
    ssd1306_write_command(0xF1);
    ssd1306_write_command(0xDB); /* Set VCOMH Deselect */
    ssd1306_write_command(0x40);
    ssd1306_write_command(0xA4); /* Entire Display On/Off */
    ssd1306_write_command(0xA6); /* Set Normal/Inverse Display */
    ssd1306_write_command(0x2E); /* Deactivate scroll */
    ssd1306_write_command(0xAF); /* Turn on display */
}

/*!
 * @brief Clears the internal frame buffer.
 */
static void ssd1306_clear_display(void)
{
    memset(g_oled_frame_buffer, 0, OLED_BUFFER_SIZE);
}

/*!
 * @brief Sets a single pixel in the internal frame buffer.
 */
static void ssd1306_draw_pixel(uint8_t x, uint8_t y)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }

    uint16_t byte_index = x + (y / 8) * OLED_WIDTH;
    uint8_t bit_index = y % 8;
    g_oled_frame_buffer[byte_index] |= (1 << bit_index);
}

/*!
 * @brief Draws a single character on the internal frame buffer.
 * It now uses FONT_SCALE_FACTOR to draw a larger character.
 */
static void ssd1306_draw_char(uint8_t x, uint8_t y, char c)
{
    if (c < ' ' || c > '~') return;

    uint8_t char_index = c - ' ';
    const uint8_t *char_data = &font5x7_data[char_index * 5];

    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t col = char_data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if ((col >> j) & 0x01)
            {
                // Draw a square of pixels for scaling
                for (uint8_t sx = 0; sx < FONT_SCALE_FACTOR; sx++)
                {
                    for (uint8_t sy = 0; sy < FONT_SCALE_FACTOR; sy++)
                    {
                        ssd1306_draw_pixel(x + i * FONT_SCALE_FACTOR + sx, y + j * FONT_SCALE_FACTOR + sy);
                    }
                }
            }
        }
    }
}

/*!
 * @brief Draws a text string on the internal frame buffer.
 */
static void ssd1306_draw_text(uint8_t x, uint8_t y, const char* text)
{
    while (*text)
    {
        ssd1306_draw_char(x, y, *text);
        x += 5 * FONT_SCALE_FACTOR + 1; // Move to the next character position, considering scaling and a 1-pixel gap
        if (x + 5 * FONT_SCALE_FACTOR >= OLED_WIDTH)
        {
            x = 0;
            y += 8 * FONT_SCALE_FACTOR;
        }
        text++;
    }
}

/*!
 * @brief Sends the entire frame buffer to the OLED.
 */
static void ssd1306_refresh_display(void)
{
    /* Set horizontal addressing mode */
    ssd1306_write_command(0x20); /* Set Memory Addressing Mode */
    ssd1306_write_command(0x00); /* Horizontal Addressing Mode */

    /* Set the column and page start/end addresses to cover the entire display */
    ssd1306_write_command(0x21); /* Set Column Address */
    ssd1306_write_command(0x00); /* Start Column */
    ssd1306_write_command(OLED_WIDTH - 1); /* End Column */

    ssd1306_write_command(0x22); /* Set Page Address */
    ssd1306_write_command(0x00); /* Start Page */
    ssd1306_write_command(OLED_HEIGHT / 8 - 1); /* End Page */

    /* The first byte is the data control byte */
    g_master_txBuff[0] = SSD1306_DATA_MODE;
    memcpy(&g_master_txBuff[1], g_oled_frame_buffer, OLED_BUFFER_SIZE);

    EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS, g_master_txBuff, OLED_BUFFER_SIZE + 1, false);
    while (!g_MasterCompletionFlag) {}
    g_MasterCompletionFlag = false;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /*Init BOARD*/
    BOARD_InitHardware();

/* DMAMux init and EDMA init */
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT)
    DMAMUX_Init(EXAMPLE_LPI2C_DMAMUX_BASEADDR);
#endif
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(edmaConfig);
#endif
    EDMA_Init(EXAMPLE_LPI2C_DMA_BASEADDR, &edmaConfig);

    /* Initialize the LPI2C master peripheral */
    EXAMPLE_I2C_MASTER.Initialize(lpi2c_master_callback);
    EXAMPLE_I2C_MASTER.PowerControl(ARM_POWER_FULL);

    /* Change the default baudrate configuration */
    EXAMPLE_I2C_MASTER.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);

    PRINTF("\r\nCMSIS LPI2C OLED example -- Master transfer.\r\n");

    /* Initialize the OLED display */
    ssd1306_init();
    ssd1306_clear_display();
    ssd1306_refresh_display();

    /* Draw "Hello, World!" on the display */
    PRINTF("Drawing text on the OLED...\r\n");

    /* Calculate the position to center the text */
    uint8_t text_width = strlen("Hello, World!") * (5 * FONT_SCALE_FACTOR + 1);
    uint8_t x_pos = (OLED_WIDTH - text_width) / 2;
    uint8_t y_pos = (OLED_HEIGHT - (8 * FONT_SCALE_FACTOR)) / 2;

    ssd1306_draw_text(x_pos, y_pos, "Hello, World!");

    ssd1306_refresh_display();

    PRINTF("Drawing complete. The text should be visible on the display.\r\n");

    while (1)
    {
    }
}
