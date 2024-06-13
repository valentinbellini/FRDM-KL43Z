################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/Drivers/Transceiver/subdir.mk
-include source/Drivers/SSD1306/subdir.mk
-include source/Drivers/PowerMode/subdir.mk
-include source/Drivers/MMA8451/subdir.mk
-include source/Drivers/Key/subdir.mk
-include source/Drivers/I2C/subdir.mk
-include source/Drivers/Board/subdir.mk
-include source/App/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := TP3_V1_BELLINI_SAITTA
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: TP3_V1_BELLINI_SAITTA.axf

# Tool invocations
TP3_V1_BELLINI_SAITTA.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="TP3_V1_BELLINI_SAITTA.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T TP3_V1_BELLINI_SAITTA_Debug.ld -o "TP3_V1_BELLINI_SAITTA.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) TP3_V1_BELLINI_SAITTA.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TP3_V1_BELLINI_SAITTA.axf"; # arm-none-eabi-objcopy -v -O binary "TP3_V1_BELLINI_SAITTA.axf" "TP3_V1_BELLINI_SAITTA.bin" ; # checksum -p MKL43Z256xxx4 -d "TP3_V1_BELLINI_SAITTA.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
