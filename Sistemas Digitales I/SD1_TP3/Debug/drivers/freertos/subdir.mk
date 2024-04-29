################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/freertos/fsl_i2c_freertos.c \
../drivers/freertos/fsl_lpuart_freertos.c \
../drivers/freertos/fsl_spi_freertos.c \
../drivers/freertos/fsl_uart_freertos.c 

C_DEPS += \
./drivers/freertos/fsl_i2c_freertos.d \
./drivers/freertos/fsl_lpuart_freertos.d \
./drivers/freertos/fsl_spi_freertos.d \
./drivers/freertos/fsl_uart_freertos.d 

OBJS += \
./drivers/freertos/fsl_i2c_freertos.o \
./drivers/freertos/fsl_lpuart_freertos.o \
./drivers/freertos/fsl_spi_freertos.o \
./drivers/freertos/fsl_uart_freertos.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/freertos/%.o: ../drivers/freertos/%.c drivers/freertos/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\board" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\source" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\freertos\freertos_kernel\include" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\freertos\freertos_kernel\portable\GCC\ARM_CM0" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\drivers" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\drivers\freertos" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\CMSIS" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\device" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\component\uart" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\component\serial_manager" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\utilities" -I"C:\Users\ivans\Documents\MCUXpressoIDE_11.8.1_1197\workspace\Trabajo_Practico_3\component\lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers-2f-freertos

clean-drivers-2f-freertos:
	-$(RM) ./drivers/freertos/fsl_i2c_freertos.d ./drivers/freertos/fsl_i2c_freertos.o ./drivers/freertos/fsl_lpuart_freertos.d ./drivers/freertos/fsl_lpuart_freertos.o ./drivers/freertos/fsl_spi_freertos.d ./drivers/freertos/fsl_spi_freertos.o ./drivers/freertos/fsl_uart_freertos.d ./drivers/freertos/fsl_uart_freertos.o

.PHONY: clean-drivers-2f-freertos

