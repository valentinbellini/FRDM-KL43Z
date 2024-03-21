################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mkl43z4.c 

C_DEPS += \
./startup/startup_mkl43z4.d 

OBJS += \
./startup/startup_mkl43z4.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\board" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\source" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\CMSIS" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\drivers" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\component\lists" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\component\uart" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\device" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\component\serial_manager" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_JERARQUICAS\utilities" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_mkl43z4.d ./startup/startup_mkl43z4.o

.PHONY: clean-startup

