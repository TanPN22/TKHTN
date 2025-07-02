################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/LoRa_lib/subdir.mk
-include Core/LIBDHT11/subdir.mk
-include Core/DHT22/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := MCB20242_Node_WSN
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
MCB20242_Node_WSN.elf \

MAP_FILES += \
MCB20242_Node_WSN.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
MCB20242_Node_WSN.list \

OBJCOPY_HEX += \
MCB20242_Node_WSN.hex \

OBJCOPY_BIN += \
MCB20242_Node_WSN.bin \

OBJCOPY_SREC += \
MCB20242_Node_WSN.srec \


# All Target
all: main-build

# Main-build Target
main-build: MCB20242_Node_WSN.elf secondary-outputs

# Tool invocations
MCB20242_Node_WSN.elf MCB20242_Node_WSN.map: $(OBJS) $(USER_OBJS) D:\DH\20242\ TK\ He\ thong\ nhung\STM32\ Homework\ Prj\MCB20242_Node_WSN\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "MCB20242_Node_WSN.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\DH\20242 TK He thong nhung\STM32 Homework Prj\MCB20242_Node_WSN\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="MCB20242_Node_WSN.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

MCB20242_Node_WSN.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "MCB20242_Node_WSN.list"
	@echo 'Finished building: $@'
	@echo ' '

MCB20242_Node_WSN.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "MCB20242_Node_WSN.hex"
	@echo 'Finished building: $@'
	@echo ' '

MCB20242_Node_WSN.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "MCB20242_Node_WSN.bin"
	@echo 'Finished building: $@'
	@echo ' '

MCB20242_Node_WSN.srec: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O srec $(EXECUTABLES) "MCB20242_Node_WSN.srec"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) MCB20242_Node_WSN.bin MCB20242_Node_WSN.elf MCB20242_Node_WSN.hex MCB20242_Node_WSN.list MCB20242_Node_WSN.map MCB20242_Node_WSN.srec default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN) $(OBJCOPY_SREC)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
