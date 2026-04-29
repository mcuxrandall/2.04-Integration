/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "pin_mux.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
// LED blinky
#define BOARD_LED_GPIO     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

// oled
#define EXAMPLE_I2C_MASTER         Driver_I2C2
#define EXAMPLE_LPI2C_DMA_BASEADDR (DMA0)
#define LPI2C_CLOCK_FREQUENCY      CLOCK_GetLPFlexCommClkFreq(2u)
#define EXAMPLE_LPI2C_DMA_CLOCK    kCLOCK_Dma0

// temp
#define EXAMPLE_MASTER             I3C1
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq(1)
//#define SENSOR_SLAVE_ADDR          0x4FU
#define SENSOR_SLAVE_ADDR          0x48U // internal
#define EXAMPLE_I2C_BAUDRATE 400000U
#define EXAMPLE_I3C_OD_BAUDRATE 1500000U
#define EXAMPLE_I3C_PP_BAUDRATE 4000000U
#define CONFIG_CPU_CORTEX_M 1
#define CONFIG_FLASH_BASE_ADDRESS 0x0
#define CONFIG_FLASH_DEFAULT_APP_OFFSET 0x0
#define CONFIG_HAS_FLASH_LOAD_OFFSET 1
#define CONFIG_FLASH_LOAD_OFFSET 0x0
#define CONFIG_I3C_RETRY_TIMES 0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
