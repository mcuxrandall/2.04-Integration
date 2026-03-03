#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_lpadc.h"
#include "fsl_delay.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_pwm.h"


#define AIN1_PORT	PORT2
#define AIN1_GPIO	GPIO2
#define AIN1	3U

#define AIN2_PORT	PORT2
#define AIN2_GPIO	GPIO2
#define AIN2	5U

#define PWMA_PORT	PORT2
#define PWMA_GPIO	GPIO2
#define PWMA	7U

//#define STBY_PORT	PORT0
//#define STBY_GPIO	GPIO0
//#define STBY	//29U

#define SW1_PORT	PORT0
#define SW1_GPIO	GPIO0
#define SW1		31U

#define POT_PORT	PORT2


void init_pins(void)
{
	CLOCK_EnableClock(kCLOCK_Port0); // Enable Port 0 clock
	CLOCK_EnableClock(kCLOCK_Port2); // Enable Port 5 clock

	    port_pin_config_t portConfig = {
	        kPORT_PullDisable,
	        kPORT_FastSlewRate,
	        kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable,
	        kPORT_LowDriveStrength,
	        kPORT_MuxAsGpio,
	        kPORT_UnlockRegister
	    };

	    /* configure outputs (motor control pins) */
	    gpio_pin_config_t outConfig = { kGPIO_DigitalOutput, 0 }; /* default low */
	    /* AIN1 (P0_2) */
	    PORT_SetPinConfig(AIN1_PORT, AIN1, &portConfig);
	    GPIO_PinInit(AIN1_GPIO, AIN1, &outConfig);

	    /* AIN2 (P1_21) */
	    PORT_SetPinConfig(AIN2_PORT, AIN2, &portConfig);
	    GPIO_PinInit(AIN2_GPIO, AIN2, &outConfig);

	    /* PWMA (P0_10) */
	    PORT_SetPinConfig(PWMA_PORT, PWMA, &portConfig);
	    GPIO_PinInit(PWMA_GPIO, PWMA, &outConfig);

	    /* STBY (P0_31) */
	    //PORT_SetPinConfig(STBY_PORT, STBY, &portConfig);
	    //GPIO_PinInit(STBY_GPIO, STBY, &outConfig);

	    /* configure SW1 as input with pull-up */
	    port_pin_config_t swConfig = portConfig;
	    swConfig.pullSelect = kPORT_PullUp; /* enable pull-up */
	    PORT_SetPinConfig(SW1_PORT, SW1, &swConfig);

	    gpio_pin_config_t inConfig = { kGPIO_DigitalInput, 0 };
	    GPIO_PinInit(SW1_GPIO, SW1, &inConfig);
}

void ADC_Init(void)
{
    lpadc_config_t config;
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_trigger_config_t triggerConfig;

    CLOCK_EnableClock(kCLOCK_Adc1);

    LPADC_GetDefaultConfig(&config);
    LPADC_Init(ADC1, &config);

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = 0U;
    LPADC_SetConvCommandConfig(ADC1, 1U, &commandConfig);

    LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
    triggerConfig.targetCommandId = 1U;
    LPADC_SetConvTriggerConfig(ADC1, 0U, &triggerConfig);
}

uint32_t ADC_ReadRaw(void)
{
    lpadc_conv_result_t result;

    LPADC_DoSoftwareTrigger(ADC1, 1U << 0U);

    while (!LPADC_GetConvResult(ADC1, &result, 0U))
    {
        /* wait */
    }

    return result.convValue;  /* raw ADC counts */
}

void SoftwarePWM(uint16_t dutyPercent, uint16_t period_us)
{
    uint16_t onTime = (period_us * dutyPercent) / 100;
    uint16_t offTime = period_us - onTime;

    // Drive PWMA high for onTime
    GPIO_PinWrite(PWMA_GPIO, PWMA, 1);
    delay_us(onTime);

    // Drive PWMA low for offTime
    GPIO_PinWrite(PWMA_GPIO, PWMA, 0);
    delay_us(offTime);
}

void MotorPWM_Init(void)
{
	pwm_config_t pwmConfig;
	    PWM_GetDefaultConfig(&pwmConfig);

	    // Initialize FlexPWM submodule 0
	    PWM_Init(PWM1, kPWM_Module_0, &pwmConfig);

	    // One-channel config array (A only)
	    pwm_signal_param_t chnlParams[1];
	    chnlParams[0].pwmChannel = kPWM_PwmA;       // Channel A
	    chnlParams[0].level = kPWM_HighTrue;        // Polarity
	    chnlParams[0].dutyCyclePercent = 50U;       // Initial duty
	    chnlParams[0].deadtimeValue = 0U;           // No deadtime

	    // Setup edge-aligned PWM at 1 kHz, source clock 120 MHz
	    PWM_SetupPwm(PWM1,
	                 kPWM_Module_0,
	                 chnlParams,           // array pointer
	                 1,                    // number of channels
	                 kPWM_EdgeAligned,
	                 1000U,                // frequency (Hz)
	                 120000000U);          // source clock (Hz)

	    PWM_StartTimer(PWM1, kPWM_Control_Module_0);
}

void MotorPWM_UpdateDuty(uint16_t dutyPercent)
{
	PWM_UpdatePwmDutycycle(PWM1,
	                           kPWM_Module_0,
	                           kPWM_PwmA,
	                           kPWM_EdgeAligned,
	                           dutyPercent);
	    PWM_SetPwmLdok(PWM1, kPWM_Control_Module_0, true);
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    //BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    init_pins();

    ADC_Init();

    // After BOARD_InitBootClocks(), etc.
    MotorPWM_Init();

    // In the while(1) loop, replace SoftwarePWM(...) with:

    PRINTF("System initialized. Starting main loop...\r\n");

    while (1)
    {
    	uint32_t SW1State = GPIO_PinRead(GPIO0, SW1);
    	uint32_t adcVal = ADC_ReadRaw() >> 3;

    	uint16_t duty = (adcVal * 100U) / 4095U;

    	PRINTF("ADC Value: %u\r\n", adcVal);

        if(SW1State == 0){
        	// AIN1 = 1, AIN2 = 0; forward
        	GPIO_PinWrite(GPIO2, AIN1, 1);
        	GPIO_PinWrite(GPIO2, AIN2, 0);
        	//GPIO_PinWrite(GPIO0, PWMA, 1);
        } else{
        	// AIN1 = 0, AIN2 = 1; reverse
        	GPIO_PinWrite(GPIO2, AIN1, 0);
        	GPIO_PinWrite(GPIO2, AIN2, 1);
        	//GPIO_PinWrite(GPIO0, PWMA, 0);
        }
        MotorPWM_UpdateDuty(duty);
    }
}
