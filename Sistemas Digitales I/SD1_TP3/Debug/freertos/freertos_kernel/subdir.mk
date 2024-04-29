################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/croutine.c \
../freertos/freertos_kernel/event_groups.c \
../freertos/freertos_kernel/list.c \
../freertos/freertos_kernel/queue.c \
../freertos/freertos_kernel/stream_buffer.c \
../freertos/freertos_kernel/tasks.c \
../freertos/freertos_kernel/timers.c 

C_DEPS += \
./freertos/freertos_kernel/croutine.d \
./freertos/freertos_kernel/event_groups.d \
./freertos/freertos_kernel/list.d \
./freertos/freertos_kernel/queue.d \
./freertos/freertos_kernel/stream_buffer.d \
./freertos/freertos_kernel/tasks.d \
./freertos/freertos_kernel/timers.d 

OBJS += \
./freertos/freertos_kernel/croutine.o \
./freertos/freertos_kernel/event_groups.o \
./freertos/freertos_kernel/list.o \
./freertos/freertos_kernel/queue.o \
./freertos/freertos_kernel/stream_buffer.o \
./freertos/freertos_kernel/tasks.o \
./freertos/freertos_kernel/timers.o 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/%.o: ../freertos/freertos_kernel/%.c freertos/freertos_kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\board" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\source" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\freertos\freertos_kernel\include" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\freertos\freertos_kernel\portable\GCC\ARM_CM0" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\drivers" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\drivers\freertos" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\CMSIS" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\device" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\component\uart" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\component\serial_manager" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\utilities" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\component\lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-freertos-2f-freertos_kernel

clean-freertos-2f-freertos_kernel:
	-$(RM) ./freertos/freertos_kernel/croutine.d ./freertos/freertos_kernel/croutine.o ./freertos/freertos_kernel/event_groups.d ./freertos/freertos_kernel/event_groups.o ./freertos/freertos_kernel/list.d ./freertos/freertos_kernel/list.o ./freertos/freertos_kernel/queue.d ./freertos/freertos_kernel/queue.o ./freertos/freertos_kernel/stream_buffer.d ./freertos/freertos_kernel/stream_buffer.o ./freertos/freertos_kernel/tasks.d ./freertos/freertos_kernel/tasks.o ./freertos/freertos_kernel/timers.d ./freertos/freertos_kernel/timers.o

.PHONY: clean-freertos-2f-freertos_kernel

