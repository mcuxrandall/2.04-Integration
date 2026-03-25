#ifndef i2c_pins_H
#define i2c_pins_H

#include "pin_mux.h"

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C2_InitPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: P1, peripheral: LP_FLEXCOMM2, signal: LPFLEXCOMM_P0, pin_signal: PIO4_0/WUU0_IN18/TRIG_IN6/FC2_P0/CT_INP16/EZH_PIO24/PLU_IN0/SINC0_MCLK3, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: up, pull_enable: enable, input_buffer: enable, invert_input: normal}
  - {pin_num: P2, peripheral: LP_FLEXCOMM2, signal: LPFLEXCOMM_P1, pin_signal: PIO4_1/TRIG_IN7/FC2_P1/CT_INP17/EZH_PIO25/PLU_IN1, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: up, pull_enable: enable, input_buffer: enable, invert_input: normal}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C2_InitPins(void)
{
//    /* Enables the clock for PORT4: Enables clock */
//    CLOCK_EnableClock(kCLOCK_Port4);
//
//    const port_pin_config_t port4_0_pinP1_config = {/* Internal pull-up resistor is enabled */
//                                                    .pullSelect = kPORT_PullUp,
//                                                    /* Low internal pull resistor value is selected. */
//                                                    .pullValueSelect = kPORT_LowPullResistor,
//                                                    /* Fast slew rate is configured */
//                                                    .slewRate = kPORT_FastSlewRate,
//                                                    /* Passive input filter is disabled */
//                                                    .passiveFilterEnable = kPORT_PassiveFilterDisable,
//                                                    /* Open drain output is disabled */
//                                                    .openDrainEnable = kPORT_OpenDrainDisable,
//                                                    /* Low drive strength is configured */
//                                                    .driveStrength = kPORT_LowDriveStrength,
//                                                    /* Pin is configured as FC2_P0 */
//                                                    .mux = kPORT_MuxAlt2,
//                                                    /* Digital input enabled */
//                                                    .inputBuffer = kPORT_InputBufferEnable,
//                                                    /* Digital input is not inverted */
//                                                    .invertInput = kPORT_InputNormal,
//                                                    /* Pin Control Register fields [15:0] are not locked */
//                                                    .lockRegister = kPORT_UnlockRegister};
//    /* PORT4_0 (pin P1) is configured as FC2_P0 */
//    PORT_SetPinConfig(PORT4, 0U, &port4_0_pinP1_config);
//
//    const port_pin_config_t port4_1_pinP2_config = {/* Internal pull-up resistor is enabled */
//                                                    .pullSelect = kPORT_PullUp,
//                                                    /* Low internal pull resistor value is selected. */
//                                                    .pullValueSelect = kPORT_LowPullResistor,
//                                                    /* Fast slew rate is configured */
//                                                    .slewRate = kPORT_FastSlewRate,
//                                                    /* Passive input filter is disabled */
//                                                    .passiveFilterEnable = kPORT_PassiveFilterDisable,
//                                                    /* Open drain output is disabled */
//                                                    .openDrainEnable = kPORT_OpenDrainDisable,
//                                                    /* Low drive strength is configured */
//                                                    .driveStrength = kPORT_LowDriveStrength,
//                                                    /* Pin is configured as FC2_P1 */
//                                                    .mux = kPORT_MuxAlt2,
//                                                    /* Digital input enabled */
//                                                    .inputBuffer = kPORT_InputBufferEnable,
//                                                    /* Digital input is not inverted */
//                                                    .invertInput = kPORT_InputNormal,
//                                                    /* Pin Control Register fields [15:0] are not locked */
//                                                    .lockRegister = kPORT_UnlockRegister};
//    /* PORT4_1 (pin P2) is configured as FC2_P1 */
//    PORT_SetPinConfig(PORT4, 1U, &port4_1_pinP2_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C2_DeinitPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: P1, peripheral: GPIO4, signal: 'GPIO, 0', pin_signal: PIO4_0/WUU0_IN18/TRIG_IN6/FC2_P0/CT_INP16/EZH_PIO24/PLU_IN0/SINC0_MCLK3, eft_interrupt: no_init,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: P2, peripheral: GPIO4, signal: 'GPIO, 1', pin_signal: PIO4_1/TRIG_IN7/FC2_P1/CT_INP17/EZH_PIO25/PLU_IN1, eft_interrupt: no_init, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C2_DeinitPins(void)
{
//    /* Enables the clock for PORT4: Enables clock */
//    CLOCK_EnableClock(kCLOCK_Port4);
//
//    const port_pin_config_t port4_0_pinP1_config = {/* Internal pull-up/down resistor is disabled */
//                                                    .pullSelect = kPORT_PullDisable,
//                                                    /* Low internal pull resistor value is selected. */
//                                                    .pullValueSelect = kPORT_LowPullResistor,
//                                                    /* Fast slew rate is configured */
//                                                    .slewRate = kPORT_FastSlewRate,
//                                                    /* Passive input filter is disabled */
//                                                    .passiveFilterEnable = kPORT_PassiveFilterDisable,
//                                                    /* Open drain output is disabled */
//                                                    .openDrainEnable = kPORT_OpenDrainDisable,
//                                                    /* Low drive strength is configured */
//                                                    .driveStrength = kPORT_LowDriveStrength,
//                                                    /* Pin is configured as PIO4_0 */
//                                                    .mux = kPORT_MuxAlt0,
//                                                    /* Digital input enabled */
//                                                    .inputBuffer = kPORT_InputBufferEnable,
//                                                    /* Digital input is not inverted */
//                                                    .invertInput = kPORT_InputNormal,
//                                                    /* Pin Control Register fields [15:0] are not locked */
//                                                    .lockRegister = kPORT_UnlockRegister};
//    /* PORT4_0 (pin P1) is configured as PIO4_0 */
//    PORT_SetPinConfig(PORT4, 0U, &port4_0_pinP1_config);
//
//    const port_pin_config_t port4_1_pinP2_config = {/* Internal pull-up/down resistor is disabled */
//                                                    .pullSelect = kPORT_PullDisable,
//                                                    /* Low internal pull resistor value is selected. */
//                                                    .pullValueSelect = kPORT_LowPullResistor,
//                                                    /* Fast slew rate is configured */
//                                                    .slewRate = kPORT_FastSlewRate,
//                                                    /* Passive input filter is disabled */
//                                                    .passiveFilterEnable = kPORT_PassiveFilterDisable,
//                                                    /* Open drain output is disabled */
//                                                    .openDrainEnable = kPORT_OpenDrainDisable,
//                                                    /* Low drive strength is configured */
//                                                    .driveStrength = kPORT_LowDriveStrength,
//                                                    /* Pin is configured as PIO4_1 */
//                                                    .mux = kPORT_MuxAlt0,
//                                                    /* Digital input enabled */
//                                                    .inputBuffer = kPORT_InputBufferEnable,
//                                                    /* Digital input is not inverted */
//                                                    .invertInput = kPORT_InputNormal,
//                                                    /* Pin Control Register fields [15:0] are not locked */
//                                                    .lockRegister = kPORT_UnlockRegister};
//    /* PORT4_1 (pin P2) is configured as PIO4_1 */
//    PORT_SetPinConfig(PORT4, 1U, &port4_1_pinP2_config);
}

#endif
