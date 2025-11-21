################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/hardware_init.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/hardware_init.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/hardware_init.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -DSDK_OS_BAREMETAL -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/drivers" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/CMSIS" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/CMSIS/m-profile" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/CMSIS_driver/Include" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/device" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/device/periph" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/utilities" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/utilities/str" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/utilities/debug_console_lite" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/component/uart" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/board" -I"/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/source" -Os -fno-common -g -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "/Users/andrew/Documents/MCUXpressoIDE_25.6.136/workspace/frdmmcxn947_cmsis_lpspi_edma_b2b_transfer_master_cm33_core0/source/mcux_config.h" -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/hardware_init.d ./board/hardware_init.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

