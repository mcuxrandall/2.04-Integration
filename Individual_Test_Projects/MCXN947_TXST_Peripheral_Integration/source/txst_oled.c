/*
 * oled.c
 *
 * SSD1306 OLED driver implementation.
 * Everything in this file is private except the five functions
 * declared in oled.h. Do not call ssd1306_* functions directly
 * from other modules — use the oled_* API instead.
 */

#include "txst_oled.h"

#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "app.h"
#include "fsl_lpi2c_cmsis.h"

/*******************************************************************************
 * Private definitions
 ******************************************************************************/
uint32_t LPI2C2_GetFreq(void)
{
    return CLOCK_GetLPFlexCommClkFreq(2U);
}

#define OLED_I2C_ADDRESS           (0x3CU)
#define OLED_BUFFER_SIZE           (OLED_WIDTH * OLED_PAGES)    /* 512 bytes — 128x32 panel */

#define SSD1306_COMMAND_MODE       (0x00U)
#define SSD1306_DATA_MODE          (0x40U)

#define I2C_TIMEOUT_TICKS          (SystemCoreClock / 10U)

/*******************************************************************************
 * Font data — 5x7, ASCII 0x20 ' ' through 0x7E '~' plus DEL
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
    0x00, 0x08, 0x14, 0x22, 0x41, //
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
 ******************************************************************************/
static const uint8_t ssd1306_init_cmds[] = {
    0xAE,                   /* Display OFF                          */
    0xA8, OLED_HEIGHT - 1,  /* Set MUX Ratio (0x1F for 32px)       */
    0xD3, 0x00,             /* Set Display Offset = 0              */
    0x60,                   /* Set Display Start Line = 32         */
    0x8D, 0x14,             /* Charge Pump ON                      */
    0xA1,                   /* Segment Remap (col 127 -> SEG0)     */
    0xC8,                   /* COM Output: remapped                */
    0xDA, 0x02,             /* COM Pins Hardware Configuration     */
    0x81, 0xCF,             /* Contrast = 0xCF                     */
    0xD9, 0xF1,             /* Pre-charge Period                   */
    0xDB, 0x40,             /* VCOMH Deselect Level                */
    0xA5,                   /* Entire Display ON (ignore RAM)      */
    0xA6,                   /* Normal (non-inverted) Display       */
    0x2E,                   /* Deactivate Scroll                   */
    0x20, 0x00,             /* Memory Addressing Mode = Horizontal */
    0xAF                    /* Display ON                          */
};

/*******************************************************************************
 * Private variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(static uint8_t g_tx_frame[OLED_BUFFER_SIZE + 1U]);
static uint8_t * const g_oled_frame_buffer = &g_tx_frame[1];
AT_NONCACHEABLE_SECTION(static uint8_t g_master_rxBuff[256U]);
AT_NONCACHEABLE_SECTION(static uint8_t g_cmd_buf[2U]);
AT_NONCACHEABLE_SECTION(static uint8_t g_win_cmd[7U]);
static volatile bool g_MasterCompletionFlag = false;
static uint8_t g_dirty_pages = 0U;

/*******************************************************************************
 * Private function prototypes
 ******************************************************************************/
static void lpi2c_master_callback(uint32_t event);
static bool i2c_wait_for_transfer(void);
static bool ssd1306_write_command(uint8_t command);
static bool ssd1306_init_display(void);
static void ssd1306_clear_display(void);
static void ssd1306_draw_pixel_internal(uint8_t x, uint8_t y);
static void ssd1306_draw_char_internal(uint8_t x, uint8_t y, char c);
static void ssd1306_draw_text_internal(uint8_t x, uint8_t y, const char *text);
static bool ssd1306_refresh_display(void);

/*******************************************************************************
 * Private functions
 ******************************************************************************/

static void lpi2c_master_callback(uint32_t event)
{
    if (event == ARM_I2C_EVENT_TRANSFER_DONE)
    {
        g_MasterCompletionFlag = true;
    }
}

static bool i2c_wait_for_transfer(void)
{
    uint32_t ticks = I2C_TIMEOUT_TICKS;
    while (!g_MasterCompletionFlag)
    {
        if (--ticks == 0U)
        {
            g_MasterCompletionFlag = false;
            PRINTF("ERROR: I2C transfer timeout!\r\n");
            return false;
        }
    }
    g_MasterCompletionFlag = false;
    return true;
}

static bool ssd1306_write_command(uint8_t command)
{
    g_cmd_buf[0] = SSD1306_COMMAND_MODE;
    g_cmd_buf[1] = command;
    EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS, g_cmd_buf, 2U, false);
    return i2c_wait_for_transfer();
}

static bool ssd1306_init_display(void)
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

static void ssd1306_clear_display(void)
{
    memset(g_oled_frame_buffer, 0U, OLED_BUFFER_SIZE);
    g_dirty_pages = (uint8_t)((1U << OLED_PAGES) - 1U);
}

static void ssd1306_draw_pixel_internal(uint8_t x, uint8_t y)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }
    uint8_t  page       = y / 8U;
    uint16_t byte_index = (uint16_t)x + (uint16_t)page * OLED_WIDTH;
    uint8_t  bit_index  = y % 8U;
    g_oled_frame_buffer[byte_index] |= (uint8_t)(1U << bit_index);
    g_dirty_pages |= (uint8_t)(1U << page);
}

static void ssd1306_draw_char_internal(uint8_t x, uint8_t y, char c)
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
                ssd1306_draw_pixel_internal((uint8_t)(x + col), (uint8_t)(y + row));
            }
        }
    }
}

static void ssd1306_draw_text_internal(uint8_t x, uint8_t y, const char *text)
{
    while (*text != '\0')
    {
        ssd1306_draw_char_internal(x, y, *text);
        x = (uint8_t)(x + 6U);
        if ((x + 5U) >= OLED_WIDTH)
        {
            x = 0U;
            y = (uint8_t)(y + 8U);
        }
        text++;
    }
}

static bool ssd1306_refresh_display(void)
{
    if (g_dirty_pages == 0U)
    {
        return true;
    }

    g_win_cmd[0] = SSD1306_COMMAND_MODE;
    g_win_cmd[1] = 0x21U;
    g_win_cmd[2] = 0x00U;
    g_win_cmd[3] = 0x7FU;
    g_win_cmd[4] = 0x22U;

    uint8_t page = 0U;

    while (page < OLED_PAGES)
    {
        if ((g_dirty_pages & (uint8_t)(1U << page)) == 0U)
        {
            page++;
            continue;
        }

        uint8_t first_page = page;
        while (page < OLED_PAGES && (g_dirty_pages & (uint8_t)(1U << page)))
        {
            page++;
        }
        uint8_t last_page = page - 1U;

        g_win_cmd[5] = first_page;
        g_win_cmd[6] = last_page;

        EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS,
                                          g_win_cmd,
                                          sizeof(g_win_cmd),
                                          false);
        if (!i2c_wait_for_transfer())
        {
            return false;
        }

        uint16_t  pixel_offset = (uint16_t)first_page * OLED_WIDTH;
        uint16_t  pixel_count  = (uint16_t)(last_page - first_page + 1U) * OLED_WIDTH;
        uint8_t  *tx_ptr;
        uint8_t   saved_byte   = 0U;
        bool      used_prefix  = false;

        if (first_page == 0U)
        {
            tx_ptr = &g_tx_frame[0];
        }
        else
        {
            tx_ptr      = &g_tx_frame[pixel_offset];
            saved_byte  = *tx_ptr;
            *tx_ptr     = SSD1306_DATA_MODE;
            used_prefix = true;
        }

        EXAMPLE_I2C_MASTER.MasterTransmit(OLED_I2C_ADDRESS,
                                          tx_ptr,
                                          pixel_count + 1U,
                                          false);
        if (!i2c_wait_for_transfer())
        {
            if (used_prefix) { *tx_ptr = saved_byte; }
            return false;
        }

        if (used_prefix)
        {
            *tx_ptr = saved_byte;
        }
    }

    g_dirty_pages = 0U;
    return true;
}

/*******************************************************************************
 * Public API
 ******************************************************************************/

bool oled_init(void)
{
    PRINTF("FLEXCOMM2 clock: %u\r\n", CLOCK_GetLPFlexCommClkFreq(2u));

#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT)
    DMAMUX_Init(EXAMPLE_LPI2C_DMAMUX_BASEADDR);
#endif

    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(edmaConfig);
#endif
    EDMA_Init(EXAMPLE_LPI2C_DMA_BASEADDR, &edmaConfig);

    NVIC_SetPriority(LP_FLEXCOMM2_IRQn, 3U);
    EnableIRQ(LP_FLEXCOMM2_IRQn);

    EXAMPLE_I2C_MASTER.Initialize(lpi2c_master_callback);
    EXAMPLE_I2C_MASTER.PowerControl(ARM_POWER_FULL);
    EXAMPLE_I2C_MASTER.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);

    /* Zero entire buffer and set DATA_MODE byte */
    memset(g_tx_frame, 0U, sizeof(g_tx_frame));
    g_tx_frame[0] = SSD1306_DATA_MODE;

    PRINTF("Scanning I2C bus...\r\n");
    for (uint8_t addr = 1; addr < 128; addr++)
    {
        uint8_t dummy = 0;
        int32_t result = EXAMPLE_I2C_MASTER.MasterTransmit(addr, &dummy, 1U, false);
        uint32_t ticks = 1000000U;
        while (ticks--);
        if (result == ARM_DRIVER_OK)
        {
            PRINTF("Found device at 0x%02X\r\n", addr);
        }
    }
    PRINTF("Scan done.\r\n");

    /* Init display — 0xA5 in cmd table forces all pixels ON during init */
    bool result = ssd1306_init_display();

    /* Push a blank frame then switch back to RAM mode */
    ssd1306_clear_display();
    ssd1306_refresh_display();
    ssd1306_write_command(0xA4);   /* back to normal RAM display mode */

    return result;
}

void oled_clear(void)
{
    ssd1306_clear_display();
}

void oled_draw_pixel(uint8_t x, uint8_t y)
{
    ssd1306_draw_pixel_internal(x, y);
}

void oled_draw_char(uint8_t x, uint8_t y, char c)
{
    ssd1306_draw_char_internal(x, y, c);
}

void oled_draw_text(uint8_t x, uint8_t y, const char *text)
{
    ssd1306_draw_text_internal(x, y, text);
}

bool oled_refresh(void)
{
    return ssd1306_refresh_display();
}
