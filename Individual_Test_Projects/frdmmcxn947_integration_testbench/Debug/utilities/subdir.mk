################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c 

S_UPPER_SRCS += \
../utilities/fsl_memcpy.S 

C_DEPS += \
./utilities/fsl_assert.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_memcpy.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DMCUXPRESSO_SDK -DCPU_MCXN947VDF_cm33_core0 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\drivers" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS\m-profile" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device\periph" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\str" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\debug_console_lite" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\component\uart" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS_driver\Include" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\board" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\source" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/%.o: ../utilities/%.S utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\drivers" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS\m-profile" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device\periph" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\str" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\debug_console_lite" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\component\uart" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS_driver\Include" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\board" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\source" -g3 -gdwarf-4 -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_memcpy.o

.PHONY: clean-utilities

