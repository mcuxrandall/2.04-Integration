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
 * @file    txst_imu.h
 * @brief   Programs the FRDM-MCXN947 to read from an Inertia Module Unit (LSM6DSOTR) over I3C.
 */

#ifndef TXST_IMU_H
#define TXST_IMU_H

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

static int txst_i3c_init (void);

#endif
