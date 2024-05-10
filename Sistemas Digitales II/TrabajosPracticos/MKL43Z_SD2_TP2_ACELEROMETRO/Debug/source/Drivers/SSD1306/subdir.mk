################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Drivers/SSD1306/font5x7.c \
../source/Drivers/SSD1306/oled.c 

C_DEPS += \
./source/Drivers/SSD1306/font5x7.d \
./source/Drivers/SSD1306/oled.d 

OBJS += \
./source/Drivers/SSD1306/font5x7.o \
./source/Drivers/SSD1306/oled.o 


# Each subdirectory must supply rules for building sources it contributes
source/Drivers/SSD1306/%.o: ../source/Drivers/SSD1306/%.c source/Drivers/SSD1306/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\board" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\source" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\drivers" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\component\uart" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\component\serial_manager" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\utilities" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\component\lists" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\device" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP2_V1\CMSIS" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-Drivers-2f-SSD1306

clean-source-2f-Drivers-2f-SSD1306:
	-$(RM) ./source/Drivers/SSD1306/font5x7.d ./source/Drivers/SSD1306/font5x7.o ./source/Drivers/SSD1306/oled.d ./source/Drivers/SSD1306/oled.o

.PHONY: clean-source-2f-Drivers-2f-SSD1306

