################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Utilities/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Middle/sensor/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Middle/rtos/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Middle/motor/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Middle/led/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Middle/buzzer/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Middle/button/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Drivers/STM32F10x_StdPeriph_Driver/src/subdir.mk
-include SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/subdir.mk
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


BUILD_ARTIFACT_NAME := Config_UART
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Config_UART.elf \

MAP_FILES += \
Config_UART.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Config_UART.list \


# All Target
all: main-build

# Main-build Target
main-build: Config_UART.elf secondary-outputs

# Tool invocations
Config_UART.elf Config_UART.map: $(OBJS) $(USER_OBJS) D:\DH\20242\ TK\ He\ thong\ nhung\STM32\ Homework\ Prj\Config_UART\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Config_UART.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\DH\20242 TK He thong nhung\STM32 Homework Prj\Config_UART\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Config_UART.map" -Wl,--gc-sections -static -L"D:\DH\Lumi\LabROBI\Config_UART\SDK_ROBI_STM32F1_1.0.0\lib_stm" --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Config_UART.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Config_UART.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Config_UART.elf Config_UART.list Config_UART.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
