################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/str/fsl_str.c 

C_DEPS += \
./utilities/str/fsl_str.d 

OBJS += \
./utilities/str/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/str/%.o: ../utilities/str/%.c utilities/str/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DMCUXPRESSO_SDK -DCPU_MCXN947VDF_cm33_core0 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\drivers" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS\m-profile" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\device\periph" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\str" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\utilities\debug_console_lite" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\component\uart" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\CMSIS_driver\Include" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\board" -I"C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\source" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\Stephen\Documents\MCUXpressoIDE_25.6.136\workspace_2026_1\frdmmcxn947_integration_testbench\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities-2f-str

clean-utilities-2f-str:
	-$(RM) ./utilities/str/fsl_str.d ./utilities/str/fsl_str.o

.PHONY: clean-utilities-2f-str

