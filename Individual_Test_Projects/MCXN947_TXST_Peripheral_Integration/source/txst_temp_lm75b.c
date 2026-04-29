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
 * @file    txst_temp_lm75b.c
 * @brief   Programs the FRDM-MCXN947 to read an LM75B temperature sensor over I3C.
 */


/*
 * Copyright 2022, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>

/* drivers */
#include "fsl_debug_console.h"

/* board*/
#include "board.h"
#include "app.h"
#include "txst_temp_lm75b.h"
