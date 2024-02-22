################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\source" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\drivers" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\utilities" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\device" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\component\uart" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\component\serial_manager" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\component\lists" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\CMSIS" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\freertos\freertos_kernel\include" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\board" -I"C:\Users\guion\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_hello\segger_sysview" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

