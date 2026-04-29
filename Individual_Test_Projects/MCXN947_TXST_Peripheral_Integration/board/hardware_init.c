/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "peripherals.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockFRO12M();

    /* Debug UART */
    BOARD_InitDebugConsole();

    /* OLED I2C clock MUST be valid */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
    CLOCK_EnableClock(kCLOCK_LPFlexComm2);

    /* DMA for OLED */
    CLOCK_EnableClock(kCLOCK_Dma0);
}
/*${function:end}*/
