################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/MKL43Z4_SD2_MEF_P5.c \
../source/SD2_board.c \
../source/key.c \
../source/mefLed.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/MKL43Z4_SD2_MEF_P5.d \
./source/SD2_board.d \
./source/key.d \
./source/mefLed.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/MKL43Z4_SD2_MEF_P5.o \
./source/SD2_board.o \
./source/key.o \
./source/mefLed.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\board" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\source" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\CMSIS" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\drivers" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\component\lists" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\component\uart" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\device" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\component\serial_manager" -I"C:\Users\valen\Documents\MCUXpressoIDE_11.9.0_2144\workspace\MKL43Z4_SD2_MEF_P5\utilities" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/MKL43Z4_SD2_MEF_P5.d ./source/MKL43Z4_SD2_MEF_P5.o ./source/SD2_board.d ./source/SD2_board.o ./source/key.d ./source/key.o ./source/mefLed.d ./source/mefLed.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

