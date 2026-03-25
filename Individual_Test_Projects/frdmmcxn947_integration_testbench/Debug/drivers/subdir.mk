################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_ctimer.c \
../drivers/fsl_edma.c \
../drivers/fsl_edma_soc.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpadc.c \
../drivers/fsl_lpflexcomm.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpi2c_cmsis.c \
../drivers/fsl_lpi2c_edma.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_pwm.c \
../drivers/fsl_reset.c \
../drivers/fsl_spc.c \
../drivers/fsl_vref.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_ctimer.d \
./drivers/fsl_edma.d \
./drivers/fsl_edma_soc.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpadc.d \
./drivers/fsl_lpflexcomm.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpi2c_cmsis.d \
./drivers/fsl_lpi2c_edma.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_pwm.d \
./drivers/fsl_reset.d \
./drivers/fsl_spc.d \
./drivers/fsl_vref.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_ctimer.o \
./drivers/fsl_edma.o \
./drivers/fsl_edma_soc.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpadc.o \
./drivers/fsl_lpflexcomm.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpi2c_cmsis.o \
./drivers/fsl_lpi2c_edma.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_pwm.o \
./drivers/fsl_reset.o \
./drivers/fsl_spc.o \
./drivers/fsl_vref.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DMCUXPRESSO_SDK -DCPU_MCXN947VDF_cm33_core0 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\drivers" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS\m-profile" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device\periph" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\str" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\debug_console_lite" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\component\uart" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS_driver\Include" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\board" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\source" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_ctimer.d ./drivers/fsl_ctimer.o ./drivers/fsl_edma.d ./drivers/fsl_edma.o ./drivers/fsl_edma_soc.d ./drivers/fsl_edma_soc.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpadc.d ./drivers/fsl_lpadc.o ./drivers/fsl_lpflexcomm.d ./drivers/fsl_lpflexcomm.o ./drivers/fsl_lpi2c.d ./drivers/fsl_lpi2c.o ./drivers/fsl_lpi2c_cmsis.d ./drivers/fsl_lpi2c_cmsis.o ./drivers/fsl_lpi2c_edma.d ./drivers/fsl_lpi2c_edma.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_pwm.d ./drivers/fsl_pwm.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_spc.d ./drivers/fsl_spc.o ./drivers/fsl_vref.d ./drivers/fsl_vref.o

.PHONY: clean-drivers

