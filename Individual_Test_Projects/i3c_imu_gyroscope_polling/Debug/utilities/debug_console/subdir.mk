################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/debug_console/fsl_debug_console.c 

C_DEPS += \
./utilities/debug_console/fsl_debug_console.d 

OBJS += \
./utilities/debug_console/fsl_debug_console.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/debug_console/%.o: ../utilities/debug_console/%.c utilities/debug_console/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -DSDK_I3C_BASED_COMPONENT_USED=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/board" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/source" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/drivers" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/device" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/utilities/debug_console" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/component/uart" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/utilities/debug_console/config" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/component/serial_manager" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/component/lists" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/device/periph" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/utilities" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/CMSIS" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/CMSIS/m-profile" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/utilities/str" -I"/Users/andrew/Documents/GitHub/1.04-Integration/individual_test_projects/i3c_imu_gyroscope_polling/component/i2c" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities-2f-debug_console

clean-utilities-2f-debug_console:
	-$(RM) ./utilities/debug_console/fsl_debug_console.d ./utilities/debug_console/fsl_debug_console.o

.PHONY: clean-utilities-2f-debug_console

