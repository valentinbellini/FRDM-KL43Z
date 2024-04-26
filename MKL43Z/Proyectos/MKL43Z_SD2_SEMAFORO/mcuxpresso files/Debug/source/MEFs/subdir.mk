################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/MEFs/mef_habitual.c \
../source/MEFs/mef_modo.c \
../source/MEFs/mef_peaton.c \
../source/MEFs/mef_secundario.c 

C_DEPS += \
./source/MEFs/mef_habitual.d \
./source/MEFs/mef_modo.d \
./source/MEFs/mef_peaton.d \
./source/MEFs/mef_secundario.d 

OBJS += \
./source/MEFs/mef_habitual.o \
./source/MEFs/mef_modo.o \
./source/MEFs/mef_peaton.o \
./source/MEFs/mef_secundario.o 


# Each subdirectory must supply rules for building sources it contributes
source/MEFs/%.o: ../source/MEFs/%.c source/MEFs/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\board" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\source" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\CMSIS" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\drivers" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\component\lists" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\component\uart" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\device" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\component\serial_manager" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\TP1_SD2_BELLINI_SAITTA\utilities" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-MEFs

clean-source-2f-MEFs:
	-$(RM) ./source/MEFs/mef_habitual.d ./source/MEFs/mef_habitual.o ./source/MEFs/mef_modo.d ./source/MEFs/mef_modo.o ./source/MEFs/mef_peaton.d ./source/MEFs/mef_peaton.o ./source/MEFs/mef_secundario.d ./source/MEFs/mef_secundario.o

.PHONY: clean-source-2f-MEFs

