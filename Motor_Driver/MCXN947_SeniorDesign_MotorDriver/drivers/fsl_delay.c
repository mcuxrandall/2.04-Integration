#include "fsl_delay.h"
#include "fsl_common.h"
#include "fsl_clock.h"

void delay_init(void)
{
    /* nothing required for SDK_DelayAtLeastUs */
}

void delay_ms(uint32_t ms)
{
    SDK_DelayAtLeastUs((uint64_t)ms * 1000ULL, CLOCK_GetFreq(kCLOCK_CoreSysClk));
}

void delay_us(uint32_t us)
{
    SDK_DelayAtLeastUs((uint64_t)us, CLOCK_GetFreq(kCLOCK_CoreSysClk));
}
