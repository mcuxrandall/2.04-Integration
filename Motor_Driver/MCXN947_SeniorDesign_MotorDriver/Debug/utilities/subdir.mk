################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c 

C_DEPS += \
./utilities/fsl_assert.d 

OBJS += \
./utilities/fsl_assert.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\board" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\source" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\drivers" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\device" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities\debug_console" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\component\uart" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities\debug_console\config" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\component\serial_manager" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\component\lists" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\device\periph" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\CMSIS" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\CMSIS\m-profile" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_SeniorDesign_MotorDriver\utilities\str" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o

.PHONY: clean-utilities

