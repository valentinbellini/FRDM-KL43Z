################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/SD2_I2C.c \
../source/SD2_board.c \
../source/ejemplo_UART0_mma8451_DMA.c \
../source/mma8451.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/uart0_drv.c 

OBJS += \
./source/SD2_I2C.o \
./source/SD2_board.o \
./source/ejemplo_UART0_mma8451_DMA.o \
./source/mma8451.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/uart0_drv.o 

C_DEPS += \
./source/SD2_I2C.d \
./source/SD2_board.d \
./source/ejemplo_UART0_mma8451_DMA.d \
./source/mma8451.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/uart0_drv.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\board" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\source" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\drivers" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\component\uart" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\component\serial_manager" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\utilities" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\component\lists" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\device" -I"C:\Users\guido\Documents\MCUXpressoIDE_11.3.0_5222\workspace\Docencia SD2\Ejemplo_DMA_UART0\CMSIS" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


