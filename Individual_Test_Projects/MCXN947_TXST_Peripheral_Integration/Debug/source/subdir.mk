################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/txst_adc.c \
../source/txst_imu.c \
../source/txst_oled.c \
../source/txst_peripheral_integration_main.c \
../source/txst_rgb.c \
../source/txst_sd_card.c \
../source/txst_temp_lm75b.c \
../source/txst_temp_p3t1755.c 

C_DEPS += \
./source/txst_adc.d \
./source/txst_imu.d \
./source/txst_oled.d \
./source/txst_peripheral_integration_main.d \
./source/txst_rgb.d \
./source/txst_sd_card.d \
./source/txst_temp_lm75b.d \
./source/txst_temp_p3t1755.d 

OBJS += \
./source/txst_adc.o \
./source/txst_imu.o \
./source/txst_oled.o \
./source/txst_peripheral_integration_main.o \
./source/txst_rgb.o \
./source/txst_sd_card.o \
./source/txst_temp_lm75b.o \
./source/txst_temp_p3t1755.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_BAREMETAL -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\drivers" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\device" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\utilities\debug_console" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\component\uart" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\utilities\debug_console\config" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\component\serial_manager" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\component\lists" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\device\periph" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\utilities" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\CMSIS" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\CMSIS\m-profile" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\utilities\str" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\CMSIS_driver\Include" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\board" -I"C:\Users\sport\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_TXST_Peripheral_Integration\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/txst_adc.d ./source/txst_adc.o ./source/txst_imu.d ./source/txst_imu.o ./source/txst_oled.d ./source/txst_oled.o ./source/txst_peripheral_integration_main.d ./source/txst_peripheral_integration_main.o ./source/txst_rgb.d ./source/txst_rgb.o ./source/txst_sd_card.d ./source/txst_sd_card.o ./source/txst_temp_lm75b.d ./source/txst_temp_lm75b.o ./source/txst_temp_p3t1755.d ./source/txst_temp_p3t1755.o

.PHONY: clean-source

