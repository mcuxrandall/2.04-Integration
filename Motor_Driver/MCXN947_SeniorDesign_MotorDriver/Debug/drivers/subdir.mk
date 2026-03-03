################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_delay.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpadc.c \
../drivers/fsl_lpflexcomm.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpspi.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_pwm.c \
../drivers/fsl_reset.c \
../drivers/fsl_spc.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_delay.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpadc.d \
./drivers/fsl_lpflexcomm.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpspi.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_pwm.d \
./drivers/fsl_reset.d \
./drivers/fsl_spc.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_delay.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpadc.o \
./drivers/fsl_lpflexcomm.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpspi.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_pwm.o \
./drivers/fsl_reset.o \
./drivers/fsl_spc.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\board" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\source" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\drivers" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\device" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities\debug_console" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\component\uart" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities\debug_console\config" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\component\serial_manager" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\component\lists" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\device\periph" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\CMSIS" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\CMSIS\m-profile" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities\str" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_delay.d ./drivers/fsl_delay.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpadc.d ./drivers/fsl_lpadc.o ./drivers/fsl_lpflexcomm.d ./drivers/fsl_lpflexcomm.o ./drivers/fsl_lpi2c.d ./drivers/fsl_lpi2c.o ./drivers/fsl_lpspi.d ./drivers/fsl_lpspi.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_pwm.d ./drivers/fsl_pwm.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_spc.d ./drivers/fsl_spc.o

.PHONY: clean-drivers

