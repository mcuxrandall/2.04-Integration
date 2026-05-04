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
 * @file    txst_adc.h
 * @brief   Programs the FRDM-MCXN947 to read from a potentiometer (P120PK-Y25BR10K) as an ADC input.
 */

#ifndef TXST_ADC_H
#define TXST_ADC_H

void adc_init(void);
uint16_t adc_read_pot(void);

#endif

