################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/lpuart_adapter.c 

C_DEPS += \
./component/uart/lpuart_adapter.d 

OBJS += \
./component/uart/lpuart_adapter.o 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c component/uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\board" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\source" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\drivers" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\component\uart" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\component\serial_manager" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\utilities" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\component\lists" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\device" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.6.1_8255\workspace\Docencia SD2\SD2_ssd1306\CMSIS" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-uart

clean-component-2f-uart:
	-$(RM) ./component/uart/lpuart_adapter.d ./component/uart/lpuart_adapter.o

.PHONY: clean-component-2f-uart

