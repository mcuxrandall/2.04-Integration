################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\drivers" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\device" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities\debug_console" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\component\uart" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities\debug_console\config" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\component\serial_manager" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\component\lists" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\device\periph" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\CMSIS" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\CMSIS\m-profile" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\utilities\str" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\board" -I"C:\Users\mccof\Documents\MCUXpressoIDE_25.6.136\workspace\MCXN947_D2_MotorDriver\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

