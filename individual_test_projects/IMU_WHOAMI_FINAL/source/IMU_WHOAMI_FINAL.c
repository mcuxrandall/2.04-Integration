/*
 * Copyright 2019, 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i3c.h"
#include "fsl_reset.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_MASTER             I3C1
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq(kCLOCK_I3c1)

#define I3C_DATA_LENGTH            1U

#define LSM6DSO_WHOAMI_REG_ADDR    0x0FU
#define LSM6DSO_WHOAMI_VALUE       0x6CU
//#define LSM6DSO_STATIC_ADDR        0x6AU  /* SA0 = GND */
#define LSM6DSO_STATIC_ADDR  0x6A

/*******************************************************************************
 * SysTick delay support
 ******************************************************************************/
static volatile uint32_t DelayTimerTick = 0;

static void InitSysTick(void)
{
    if (SysTick_Config(SystemCoreClock / 1000000U))
    {
        PRINTF("SysTick init failed\r\n");
        while (1) {}
    }
}

void SysTick_Handler(void)
{
    if (DelayTimerTick < 0xFFFFFFFFU)
    {
        DelayTimerTick++;
    }
}

static void Delay_us(uint32_t us)
{
    DelayTimerTick = 0;
    while (DelayTimerTick < us) {}
}

static void Delay_ms(uint32_t ms)
{
    Delay_us(ms * 1000U);
}

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    i3c_master_config_t masterConfig;
    i3c_master_transfer_t masterXfer;

    i3c_device_info_t *deviceList = NULL;
    uint8_t deviceCount = 0;

    status_t result;

    /* Board init */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    InitSysTick();

    PRINTF("\r\n=== MCXN I3C Master – LSM6DSO Demo ===\r\n");
    PRINTF("I3C clock: %u Hz\r\n", I3C_MASTER_CLOCK_FREQUENCY);

    /* I3C master config */
    I3C_MasterGetDefaultConfig(&masterConfig);
    /* Master default config */
    masterConfig.baudRate_Hz.i3cPushPullBaud  = 1000000U;  // 4 MHz
    masterConfig.baudRate_Hz.i3cOpenDrainBaud = 400000U;  // 500 kHz
    masterConfig.enableOpenDrainHigh = false;
    masterConfig.enableOpenDrainStop = false;

    I3C_MasterInit(EXAMPLE_MASTER, &masterConfig, I3C_MASTER_CLOCK_FREQUENCY);
    PRINTF("I3C master initialized\r\n");

    /* Sensor power‑up time */
    Delay_ms(100);

    /***************************************************************************
     * DAA
     **************************************************************************/
    PRINTF("\r\n--- Performing DAA ---\r\n");

    /* Static address list REQUIRED by this SDK */
    uint8_t staticAddrList[] = {
        LSM6DSO_STATIC_ADDR
    };

    i3c_master_daa_baudrate_t daaBaudRate;

    /*
     * i3cOpenDrainBaud controls the I2C/OD phase during DAA
     * LSM6DSO requires Fast‑Mode Plus (1 MHz)
     */
    daaBaudRate.i3cOpenDrainBaud = 1000000U; // FMP for LSM6DSO
    daaBaudRate.i3cPushPullBaud  = 1000000U; // Safe SDR

    result = I3C_MasterProcessDAASpecifiedBaudrate(
        EXAMPLE_MASTER,
        staticAddrList,                  /* ✅ MUST be non‑NULL */
        sizeof(staticAddrList),           /* address count */
        &daaBaudRate
    );

    if (result == kStatus_Success)
    {
        PRINTF("DAA successful\r\n");

        deviceList = I3C_MasterGetDeviceListAfterDAA(EXAMPLE_MASTER, &deviceCount);

        if (deviceCount == 0)
        {
            PRINTF("No devices found after DAA\r\n");
        }
        else
        {
            PRINTF("Found %u device(s):\r\n", deviceCount);
            for (uint8_t i = 0; i < deviceCount; i++)
            {
                PRINTF(" [%u] dynAddr=0x%02X vendor=0x%04X part=0x%08X\r\n",
                       i,
                       deviceList[i].dynamicAddr,
                       deviceList[i].vendorID,
                       deviceList[i].partNumber);
            }
        }
    }
    else
    {
        PRINTF("DAA failed (0x%x), falling back to I2C\r\n", result);
        deviceList = NULL;
        deviceCount = 0;
    }

    /***************************************************************************
     * Address selection
     **************************************************************************/
    uint8_t targetAddr;
    bool useI3C;

    if ((deviceList != NULL) && (deviceCount > 0) &&
        (deviceList[0].dynamicAddr != 0))
    {
        targetAddr = deviceList[0].dynamicAddr;
        useI3C = true;
        PRINTF("Using I3C dynamic address 0x%02X\r\n", targetAddr);
    }
    else
    {
        targetAddr = LSM6DSO_STATIC_ADDR;
        useI3C = false;
        PRINTF("Using I2C static address 0x%02X\r\n", targetAddr);
    }

    /***************************************************************************
     * WHO_AM_I read
     **************************************************************************/
    PRINTF("\r\n--- Reading WHO_AM_I ---\r\n");

    uint8_t whoami = 0;

    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress   = targetAddr;
    masterXfer.subaddress     = LSM6DSO_WHOAMI_REG_ADDR;
    masterXfer.subaddressSize = 1;
    masterXfer.direction      = kI3C_Read;
    masterXfer.data           = &whoami;
    masterXfer.dataSize       = I3C_DATA_LENGTH;
    masterXfer.flags          = kI3C_TransferRepeatedStartFlag;
    masterXfer.busType        = useI3C ? kI3C_TypeI3CSdr : kI3C_TypeI2C;

    result = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);

    if ((result == kStatus_Success) && (whoami == LSM6DSO_WHOAMI_VALUE))
    {
        PRINTF("\r\n✅ SUCCESS: WHO_AM_I = 0x%02X\r\n", whoami);
    }
    else
    {
        PRINTF("\r\n❌ FAILED\r\n");
        PRINTF(" Status : 0x%x\r\n", result);
        PRINTF(" Read   : 0x%02X (expected 0x6C)\r\n", whoami);
        PRINTF(" Bus    : %s\r\n", useI3C ? "I3C" : "I2C");
        PRINTF(" Addr   : 0x%02X\r\n", targetAddr);
        PRINTF(" Check  : SA0=GND, pull‑ups, power‑up delay\r\n");
    }

    while (1)
    {
        Delay_ms(2000);
        PRINTF(".\r\n");
    }
}
