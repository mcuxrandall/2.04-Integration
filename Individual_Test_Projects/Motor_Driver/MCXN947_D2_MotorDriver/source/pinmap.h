#ifndef PINMAP_H_
#define PINMAP_H_

#define SW1_PORT	0U
#define SW1_PIN		31U

#define PWM_FREQ_HZ 20000U
#define CTIMER_BASE CTIMER0
#define CTIMER_INDEX 0U

#define PWMA_PORT 0U
#define PWMA_PIN 24U

#define AIN1_PORT 2U
#define AIN1_PIN  7U//3U

#define AIN2_PORT 2U
#define AIN2_PIN  6U//5U

#define ADC_BASE ADC0
#define ADC_CMD  1U
#define ADC_TRIG 1U
#define ADC_CH   0U   // ADC0_A0


#endif /* PINMAP_H_ */
