################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i3c.c \
../drivers/fsl_lpflexcomm.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_reset.c \
../drivers/fsl_spc.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i3c.d \
./drivers/fsl_lpflexcomm.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_reset.d \
./drivers/fsl_spc.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i3c.o \
./drivers/fsl_lpflexcomm.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_reset.o \
./drivers/fsl_spc.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/source" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/drivers" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/CMSIS" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/CMSIS/m-profile" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/device" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/device/periph" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/utilities" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/utilities/str" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/utilities/debug_console_lite" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/component/uart" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/component/p3t1755" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/board" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0/source/mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_i3c.d ./drivers/fsl_i3c.o ./drivers/fsl_lpflexcomm.d ./drivers/fsl_lpflexcomm.o ./drivers/fsl_lpi2c.d ./drivers/fsl_lpi2c.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_spc.d ./drivers/fsl_spc.o

.PHONY: clean-drivers

