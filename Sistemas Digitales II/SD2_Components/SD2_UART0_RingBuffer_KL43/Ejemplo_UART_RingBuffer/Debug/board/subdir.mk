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
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\board" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\source" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\drivers" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\component\uart" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\component\serial_manager" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\utilities" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\component\lists" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\device" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\Ejemplo_UART_RingBuffer\CMSIS" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

