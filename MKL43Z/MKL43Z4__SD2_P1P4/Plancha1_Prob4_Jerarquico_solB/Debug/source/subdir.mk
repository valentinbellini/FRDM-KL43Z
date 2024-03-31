################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/SD2_board.c \
../source/key.c \
../source/main.c \
../source/mefModo.c \
../source/mefSW1.c \
../source/mefSec.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/SD2_board.o \
./source/key.o \
./source/main.o \
./source/mefModo.o \
./source/mefSW1.o \
./source/mefSec.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/SD2_board.d \
./source/key.d \
./source/main.d \
./source/mefModo.d \
./source/mefSW1.d \
./source/mefSec.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


