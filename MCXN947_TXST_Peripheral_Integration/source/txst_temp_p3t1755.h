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
 * @file    txst_temp.h
 * @brief   Programs the FRDM-MCXN947 to read from the board's built-in P3T1755DP temperature sensor over I2C and I3C.
 */

#ifndef TXST_TEMP_P3T1755_H
#define TXST_TEMP_P3T1755_H

#include "fsl_i3c.h"
#include "fsl_p3t1755.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I3C_TIME_OUT_INDEX 100000000U

#define SENSOR_ADDR 0x08U
#define CCC_RSTDAA  0x06U
#define CCC_SETDASA 0x87



/*******************************************************************************
 * Functions
 ******************************************************************************/

// p3t1755 temperature sensor (pre-installed in the FRDM-MCXN947)
static void i3c_master_callback(I3C_Type *base, i3c_master_handle_t *handle, status_t status, void *userData);
status_t I3C_WriteSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize);
status_t I3C_ReadSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize);
status_t p3t1755_set_dynamic_address(void);
void init_i3c_temp(void);

extern p3t1755_handle_t p3t1755Handle;

#endif
