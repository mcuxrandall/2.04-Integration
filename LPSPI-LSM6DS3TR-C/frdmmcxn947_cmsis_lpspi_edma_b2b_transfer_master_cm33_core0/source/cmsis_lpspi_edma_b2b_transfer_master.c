#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "lsm6ds3tr-c_reg.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Sensor is on SPI bus */
#define SENSOR_BUS LPSPI6

/* SPI and CS pin configuration */
#define LSM6DS3TRC_SPI_BASE       LPSPI6
#define LSM6DS3TRC_CS_GPIO GPIO1
#define LSM6DS3TRC_CS_PIN         23

#define SPI_BAUDRATE              5000U // 1 MHz SPI
#define SPI_MODE                  kLPSPI_ClockPolarityActiveLow
#define SPI_PHASE                 kLPSPI_ClockPhaseFirstEdge

#define BOOT_TIME        15 // ms
#define WAIT_TIME_A     100 // ms
#define WAIT_TIME_G_01  150 // ms
#define WAIT_TIME_G_02   50 // ms

#define MIN_ST_LIMIT_mg        90.0f
#define MAX_ST_LIMIT_mg      1700.0f
#define MIN_ST_LIMIT_mdps   150000.0f
#define MAX_ST_LIMIT_mdps   700000.0f

#define ST_PASS     1U
#define ST_FAIL     0U

/* Function prototypes */
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
static void tx_com(uint8_t *tx_buffer, uint16_t len);
static void platform_delay(uint32_t ms);
static void platform_init(void);

/* SPI and GPIO handles */
static lpspi_master_config_t masterConfig;
static lpspi_transfer_t spiTransfer;

/* Helper for CS control */
static inline void cs_low(void)
{
    GPIO_PinWrite(LSM6DS3TRC_CS_GPIO, LSM6DS3TRC_CS_PIN, 0U);
}
static inline void cs_high(void)
{
    GPIO_PinWrite(LSM6DS3TRC_CS_GPIO, LSM6DS3TRC_CS_PIN, 1U);
}

/* ------------------- LSM6DS3TR-C Self Test Example ------------------- */
void lsm6ds3tr_c_self_test(void)
{
    uint8_t tx_buffer[256];
    int16_t data_raw[3];
    stmdev_ctx_t dev_ctx;
    float val_st_off[3];
    float val_st_on[3];
    float test_val[3];
    uint8_t st_result;
    uint8_t whoamI, drdy, rst;
    uint8_t i, j;

    PRINTF("Initializing LSM6DS3TR-C self-test example...\r\n");

    /* Initialize driver interface */
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.mdelay = platform_delay;
    dev_ctx.handle = (void *)SENSOR_BUS;

    platform_init();
    platform_delay(BOOT_TIME);

    // Debug print WHOAMI after delay
    uint8_t debug_whoami = 0;
    platform_read(NULL, 0x0F, &debug_whoami, 1);
    PRINTF("WHOAMI after delay: 0x%02X\r\n", debug_whoami);

    // Proceed with driver WHOAMI check
    if (lsm6ds3tr_c_device_id_get(&dev_ctx, &whoamI) != 0) {
        PRINTF("Failed to read WHOAMI\r\n");
        while (1);
    }
    if (whoamI != LSM6DS3TR_C_ID) {
        PRINTF("LSM6DS3TR-C not found! WHOAMI=0x%02X\r\n", whoamI);
        while (1);
    }

    /* Check WHOAMI */
    lsm6ds3tr_c_device_id_get(&dev_ctx, &whoamI);
    if (whoamI != LSM6DS3TR_C_ID)
    {
        PRINTF("LSM6DS3TR-C not found! WHOAMI=0x%02X\r\n", whoamI);
        while (1);
    }
    PRINTF("Sensor detected (WHOAMI=0x%02X)\r\n", whoamI);

    /* Reset to defaults */
    lsm6ds3tr_c_reset_set(&dev_ctx, PROPERTY_ENABLE);
    do {
        lsm6ds3tr_c_reset_get(&dev_ctx, &rst);
    } while (rst);

    /* Enable BDU */
    lsm6ds3tr_c_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

    /* ---------------- Accelerometer Self Test ---------------- */
    lsm6ds3tr_c_xl_data_rate_set(&dev_ctx, LSM6DS3TR_C_XL_ODR_52Hz);
    lsm6ds3tr_c_xl_full_scale_set(&dev_ctx, LSM6DS3TR_C_4g);
    platform_delay(WAIT_TIME_A);

    /* Discard first sample */
    lsm6ds3tr_c_xl_flag_data_ready_get(&dev_ctx, &drdy);
    lsm6ds3tr_c_acceleration_raw_get(&dev_ctx, data_raw);

    memset(val_st_off, 0, sizeof(val_st_off));
    for (i = 0; i < 5; i++) {
        do {
            lsm6ds3tr_c_xl_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);
        lsm6ds3tr_c_acceleration_raw_get(&dev_ctx, data_raw);
        for (j = 0; j < 3; j++) {
            val_st_off[j] += lsm6ds3tr_c_from_fs4g_to_mg(data_raw[j]);
        }
    }
    for (i = 0; i < 3; i++) val_st_off[i] /= 5.0f;

    lsm6ds3tr_c_xl_self_test_set(&dev_ctx, LSM6DS3TR_C_XL_ST_POSITIVE);
    platform_delay(WAIT_TIME_A);

    memset(val_st_on, 0, sizeof(val_st_on));
    for (i = 0; i < 5; i++) {
        do {
            lsm6ds3tr_c_xl_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);
        lsm6ds3tr_c_acceleration_raw_get(&dev_ctx, data_raw);
        for (j = 0; j < 3; j++) {
            val_st_on[j] += lsm6ds3tr_c_from_fs4g_to_mg(data_raw[j]);
        }
    }
    for (i = 0; i < 3; i++) {
        val_st_on[i] /= 5.0f;
        test_val[i] = fabsf(val_st_on[i] - val_st_off[i]);
    }

    st_result = ST_PASS;
    for (i = 0; i < 3; i++) {
        if (test_val[i] < MIN_ST_LIMIT_mg || test_val[i] > MAX_ST_LIMIT_mg) {
            st_result = ST_FAIL;
        }
    }

    lsm6ds3tr_c_xl_self_test_set(&dev_ctx, LSM6DS3TR_C_XL_ST_DISABLE);
    lsm6ds3tr_c_xl_data_rate_set(&dev_ctx, LSM6DS3TR_C_XL_ODR_OFF);

    PRINTF("Accelerometer self-test: %s\r\n", st_result == ST_PASS ? "PASS" : "FAIL");

    /* ---------------- Gyroscope Self Test ---------------- */
    lsm6ds3tr_c_gy_data_rate_set(&dev_ctx, LSM6DS3TR_C_GY_ODR_208Hz);
    lsm6ds3tr_c_gy_full_scale_set(&dev_ctx, LSM6DS3TR_C_2000dps);
    platform_delay(WAIT_TIME_G_01);

    memset(val_st_off, 0, sizeof(val_st_off));
    for (i = 0; i < 5; i++) {
        do {
            lsm6ds3tr_c_gy_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);
        lsm6ds3tr_c_angular_rate_raw_get(&dev_ctx, data_raw);
        for (j = 0; j < 3; j++) {
            val_st_off[j] += lsm6ds3tr_c_from_fs2000dps_to_mdps(data_raw[j]);
        }
    }
    for (i = 0; i < 3; i++) val_st_off[i] /= 5.0f;

    lsm6ds3tr_c_gy_self_test_set(&dev_ctx, LSM6DS3TR_C_GY_ST_POSITIVE);
    platform_delay(WAIT_TIME_G_02);

    memset(val_st_on, 0, sizeof(val_st_on));
    for (i = 0; i < 5; i++) {
        do {
            lsm6ds3tr_c_gy_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);
        lsm6ds3tr_c_angular_rate_raw_get(&dev_ctx, data_raw);
        for (j = 0; j < 3; j++) {
            val_st_on[j] += lsm6ds3tr_c_from_fs2000dps_to_mdps(data_raw[j]);
        }
    }
    for (i = 0; i < 3; i++) {
        val_st_on[i] /= 5.0f;
        test_val[i] = fabsf(val_st_on[i] - val_st_off[i]);
    }

    for (i = 0; i < 3; i++) {
        if (test_val[i] < MIN_ST_LIMIT_mdps || test_val[i] > MAX_ST_LIMIT_mdps) {
            st_result = ST_FAIL;
        }
    }

    lsm6ds3tr_c_gy_self_test_set(&dev_ctx, LSM6DS3TR_C_GY_ST_DISABLE);
    lsm6ds3tr_c_gy_data_rate_set(&dev_ctx, LSM6DS3TR_C_GY_ODR_OFF);

    PRINTF("Gyroscope self-test: %s\r\n", st_result == ST_PASS ? "PASS" : "FAIL");
    PRINTF("\r\nLSM6DS3TR-C Self-Test Complete.\r\n");
}

/* ---------------- Platform-dependent SPI + utility functions ---------------- */

static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
    uint8_t txData[16];
    status_t status;

    cs_low();
    txData[0] = reg & 0x7F; // Write command
    spiTransfer.txData = txData;
    spiTransfer.rxData = NULL;
    spiTransfer.dataSize = 1;
    LPSPI_MasterTransferBlocking(LSM6DS3TRC_SPI_BASE, &spiTransfer);

    spiTransfer.txData = (uint8_t *)bufp;
    spiTransfer.dataSize = len;
    status = LPSPI_MasterTransferBlocking(LSM6DS3TRC_SPI_BASE, &spiTransfer);
    cs_high();

    return (status == kStatus_Success) ? 0 : -1;
}

static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    uint8_t cmd = reg | 0x80; // Read bit
    status_t status;

    cs_low();
    spiTransfer.txData = &cmd;
    spiTransfer.rxData = NULL;
    spiTransfer.dataSize = 1;
    LPSPI_MasterTransferBlocking(LSM6DS3TRC_SPI_BASE, &spiTransfer);

    spiTransfer.txData = NULL;
    spiTransfer.rxData = bufp;
    spiTransfer.dataSize = len;
    status = LPSPI_MasterTransferBlocking(LSM6DS3TRC_SPI_BASE, &spiTransfer);
    cs_high();

    return (status == kStatus_Success) ? 0 : -1;
}

static void tx_com(uint8_t *tx_buffer, uint16_t len)
{
    PRINTF("%.*s", len, tx_buffer);
}

static void platform_delay(uint32_t ms)
{
    SDK_DelayAtLeastUs(ms * 1000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
}

static void platform_init(void)
{
    gpio_pin_config_t cs_config = {kGPIO_DigitalOutput, 1};

    GPIO_PinInit(LSM6DS3TRC_CS_GPIO, LSM6DS3TRC_CS_PIN, &cs_config);
    cs_high();

    CLOCK_SetClkDiv(kCLOCK_DivSai1Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_TSI);

    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = SPI_BAUDRATE;
    masterConfig.cpol = SPI_MODE;
    masterConfig.cpha = SPI_PHASE;
    masterConfig.direction = kLPSPI_MsbFirst;
    LPSPI_MasterInit(LSM6DS3TRC_SPI_BASE, &masterConfig, CLOCK_GetFreq(kCLOCK_LPSpi6));

    /*PRINTF("Reading WHOAMI raw...\r\n");
    uint8_t who=0xFF;
    platform_read(NULL, 0x0F, &who, 1);
    PRINTF("WHOAMI = 0x%02X\r\n", who);*/
}

/* ---------------- Main ---------------- */
int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("\r\n=== LSM6DS3TR-C SPI Self-Test (MCXN947) ===\r\n");
    lsm6ds3tr_c_self_test();

    while (1)
    {
        __WFI();
    }
}
