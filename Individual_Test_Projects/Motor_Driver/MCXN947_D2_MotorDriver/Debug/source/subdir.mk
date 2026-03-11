################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/adc.c \
../source/main.c \
../source/motor.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/adc.d \
./source/main.d \
./source/motor.d \
./source/semihost_hardfault.d 

OBJS += \
./source/adc.o \
./source/main.o \
./source/motor.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\drivers" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\device" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities\debug_console" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\component\uart" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities\debug_console\config" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\component\serial_manager" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\component\lists" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\device\periph" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\CMSIS" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\CMSIS\m-profile" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities\str" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\board" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/adc.d ./source/adc.o ./source/main.d ./source/main.o ./source/motor.d ./source/motor.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

