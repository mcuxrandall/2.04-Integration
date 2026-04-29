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
 * @file    txst_sd_card.c
 * @brief   Programs the FRDM-MCXN947 to read from and write to an adafruit microsd card with SPI.
 */


/* Language: C
   Zephyr RTOS: microSD over SPI Example
   This example initializes SPI and communicates with an SD card using FatFs.
*/
//#include <zephyr.h>
//#include <device.h>
//#include <drivers/spi.h>
//#include <fs/fs.h>
//#include <ff.h>
//#include <stdio.h>

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"


//#define SPI_BUS_LABEL DT_LABEL(DT_ALIAS(spi_0))  // Adjust to your board's SPI node
//#define SPI_CS_GPIO_LABEL DT_LABEL(DT_ALIAS(cs0)) // Adjust if needed
//
//// microSD
//// Header: J6 mikroBUS
//// J6_3  /  P3_23  /  ME_FC6_SPI_CS    /  SPI chip select (CS)
//// J6_4  /  P3_21  /  ME_FC6_SPI_CLK   /  SPI serial clock port (SCK)
//// J6_5  /  P3_22  /  ME_FC6_SPI_MISO  /  SPI serial data in (MISO)
//// J6_6  /  P3_20  /  ME_FC6_SPI_MOSI  /  SPI serial data out (MOSI)
//
//
//
//// SPI configuration
//static const struct spi_config spi_cfg = {
//    .frequency = 4000000U,      // 4 MHz for initialization, can increase after
//    .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_MODE_0,
//    .slave = 0,
//    .cs = NULL
//};
//
//
//void main(void)
//{
//    const struct device *spi_dev;
//    FRESULT fr;
//    FATFS fs;
//    FIL file;
//    char buf[64];
//
//    PRINTF("microSD SPI Example with Zephyr");
//
//    // Bind SPI device
//    spi_dev = device_get_binding(SPI_BUS_LABEL);
//    if (!spi_dev) {
//    	PRINTF("Cannot find SPI device");
//        return;
//    }
//
//    // Mount the filesystem (microSD)
//    fr = f_mount(&fs, "/SD:", 1);
//    if (fr != FR_OK) {
//    	PRINTF("Failed to mount filesystem: %d", fr);
//        return;
//    }
//    PRINTF("Filesystem mounted successfully");
//
//    // Create and write a file
//    fr = f_open(&file, "/SD/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
//    if (fr == FR_OK) {
//        f_write(&file, "Hello Zephyr SD!", 17, NULL);
//        f_close(&file);
//        PRINTF("File written successfully");
//    }
//    else {
//    	PRINTF("Failed to open file: %d", fr);
//    }
//    // Read the file
//    fr = f_open(&file, "/SD/test.txt", FA_READ);
//    if (fr == FR_OK) {
//        UINT br;
//        f_read(&file, buf, sizeof(buf)-1, &br);
//        buf[br] = '\0';
//        PRINTF("Read: %s", buf);
//        f_close(&file);
//    }
//    else {
//    	PRINTF("Failed to open file for reading: %d", fr);
//    }
//}
