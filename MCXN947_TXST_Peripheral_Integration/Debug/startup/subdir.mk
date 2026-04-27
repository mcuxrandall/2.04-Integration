################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/boot_multicore_slave.c \
../startup/startup_mcxn947_cm33_core0.c 

C_DEPS += \
./startup/boot_multicore_slave.d \
./startup/startup_mcxn947_cm33_core0.d 

OBJS += \
./startup/boot_multicore_slave.o \
./startup/startup_mcxn947_cm33_core0.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_BAREMETAL -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/drivers" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/device" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/utilities/debug_console" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/component/uart" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/utilities/debug_console/config" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/component/serial_manager" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/component/lists" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/device/periph" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/utilities" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/CMSIS" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/CMSIS/m-profile" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/utilities/str" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/CMSIS_driver/Include" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/board" -I"/Users/andrew/Documents/Final_Workspace/MCXN947_TXST_Peripheral_Integration/source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/boot_multicore_slave.d ./startup/boot_multicore_slave.o ./startup/startup_mcxn947_cm33_core0.d ./startup/startup_mcxn947_cm33_core0.o

.PHONY: clean-startup

