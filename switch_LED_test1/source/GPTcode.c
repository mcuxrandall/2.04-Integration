/*
 * GPTcode.c
 *
 *  Created on: Oct 15, 2025
 *      Author: Stephen
 */


#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "app.h"
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 800000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

#define LED_PORT   1U
#define LED_PIN    21U   // ARD_D5 (P1_21) = J4-pin12

#define BUTTON_PORT 0U
#define BUTTON_PIN  29U  // ARD_D2 (P0_29) = J4-pin6

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Board pin, clock, debug console init */
    // BOARD_InitHardware();


    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0};
    gpio_pin_config_t button_config = {kGPIO_DigitalInput, 0};

    // Init GPIOs
    GPIO_PinInit(GPIO, LED_PORT, LED_PIN, &led_config);
    GPIO_PinInit(GPIO, BUTTON_PORT, BUTTON_PIN, &button_config);

    // Enable internal pull-up for the button
    IOCON_PinMuxSet(IOCON, BUTTON_PORT, BUTTON_PIN,
        IOCON_FUNC0 | IOCON_MODE_PULLUP);

    while (1)
    {
        // Read button state
        if (GPIO_PinRead(GPIO, BUTTON_PORT, BUTTON_PIN) == 0)
        {
            // Button pressed
            GPIO_PinWrite(GPIO, LED_PORT, LED_PIN, 1);
        }
        else
        {
            // Button released
            GPIO_PinWrite(GPIO, LED_PORT, LED_PIN, 0);
        }
    }
}
