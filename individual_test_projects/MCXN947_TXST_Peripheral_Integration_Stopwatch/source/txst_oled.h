/*
 * oled.h
 *
 * Public interface for the SSD1306 OLED driver.
 * Include this file in any module that needs to draw to the display.
 * All internal driver details (buffers, SSD1306 commands, font data)
 * are private to oled.c and not visible here.
 */

#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Display geometry — exposed so callers can do centering math if needed
 ******************************************************************************/
#define OLED_WIDTH    (128U)
#define OLED_HEIGHT   (64U)
#define OLED_PAGES    (OLED_HEIGHT / 8U)   /* 8 pages */
/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initializes the I2C peripheral and the SSD1306 display.
 *
 * Must be called once before any other oled_* function.
 * Handles EDMA, LPI2C, and the full SSD1306 init command sequence.
 *
 * @return true  on success.
 * @return false if any I2C transfer timed out during init.
 */
bool oled_init(void);

/*!
 * @brief Clears the internal frame buffer and marks all pages dirty.
 *
 * Does NOT push to the display — call oled_refresh() after to blank the screen.
 */
void oled_clear(void);

/*!
 * @brief Sets a single pixel in the frame buffer.
 *
 * Marks the affected page dirty. Does NOT push to the display.
 *
 * @param x  Column (0 – OLED_WIDTH-1).
 * @param y  Row    (0 – OLED_HEIGHT-1).
 */
void oled_draw_pixel(uint8_t x, uint8_t y);

/*!
 * @brief Draws a single printable ASCII character at (x, y).
 *
 * Uses the internal 5x7 font. Marks affected pages dirty.
 * Does NOT push to the display.
 *
 * @param x  Column origin (0 – OLED_WIDTH-1).
 * @param y  Row origin    (0 – OLED_HEIGHT-1), aligned to a pixel row.
 * @param c  ASCII character (0x20 ' ' – 0x7E '~').
 */
void oled_draw_char(uint8_t x, uint8_t y, char c);

/*!
 * @brief Draws a null-terminated string starting at (x, y).
 *
 * Automatically wraps to the next 8-pixel row when the right edge
 * is reached. Marks affected pages dirty. Does NOT push to the display.
 *
 * @param x     Column origin.
 * @param y     Row origin.
 * @param text  Null-terminated ASCII string.
 */
void oled_draw_text(uint8_t x, uint8_t y, const char *text);

/*!
 * @brief Transmits all dirty pages to the display over I2C/DMA.
 *
 * Only pages modified since the last refresh are sent.
 * Call this once after all drawing for a frame is complete.
 *
 * @return true  if all dirty pages were transmitted successfully.
 * @return false if an I2C transfer timed out.
 */
bool oled_refresh(void);

#endif /* OLED_H */
