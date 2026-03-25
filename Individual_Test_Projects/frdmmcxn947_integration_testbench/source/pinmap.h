#ifndef PINMAP_H_
#define PINMAP_H_



#define PWM_FREQ_HZ 20000U
#define CTIMER_BASE CTIMER0
#define CTIMER_INDEX 0U

// PWMA = PWM0, Ctimer0, match 0, header pin J2_8, output pin P0_24
#define PWMA_CTimer CTIMER0
#define PWMA_Match  kCTIMER_Match_0
#define AIN1_port GPIO2
#define AIN1_PIN  6U
#define AIN2_port GPIO2
#define AIN2_PIN  7U
#define STANDBY_port GPIO2
#define STANDBY_PIN  0U

// PWMA = PWM0, Ctimer0, match 0, header pin J2_8, output pin P0_24
#define PWMB_CTimer CTIMER0
#define PWMB_Match  kCTIMER_Match_2
#define BIN1_port GPIO2
#define BIN1_PIN  3U
#define BIN2_port GPIO1
#define BIN2_PIN  22U

#define ADC_BASE ADC0
#define ADC_CMD  1U
#define ADC_TRIG 1U
#define ADC_CH   0U   // ADC0_A0


#endif /* PINMAP_H_ */
