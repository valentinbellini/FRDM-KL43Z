################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/generic_list.c 

C_DEPS += \
./component/lists/generic_list.d 

OBJS += \
./component/lists/generic_list.o 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\board" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\source" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\drivers" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\component\uart" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\component\serial_manager" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\utilities" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\component\lists" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\device" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP3_V1_BELLINI_SAITTA\CMSIS" -Os -fno-common -g -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-lists

clean-component-2f-lists:
	-$(RM) ./component/lists/generic_list.d ./component/lists/generic_list.o

.PHONY: clean-component-2f-lists

