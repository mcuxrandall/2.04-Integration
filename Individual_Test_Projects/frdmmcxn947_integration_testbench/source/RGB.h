#ifndef RGB_H
#define RGB_H

#include "fsl_pwm.h"
#include "fsl_debug_console.h"


// PWM
#define RGBs_PWM_Type             PWM1
#define PWM_SRC_CLK_FREQ          CLOCK_GetFreq(kCLOCK_BusClk)
#define APP_DEFAULT_PWM_FREQUENCY (10000UL)

static void Init_PWM_3Ch(pwm_submodule_t submodule);
status_t initialize_PWM1(void);

#endif

