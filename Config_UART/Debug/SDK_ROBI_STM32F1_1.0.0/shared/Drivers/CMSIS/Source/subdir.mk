################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.c \
D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.c 

OBJS += \
./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.o \
./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.o 

C_DEPS += \
./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.d \
./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.o: D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.c SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F10X_MD -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Include" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/STM32F10x_StdPeriph_Driver/inc" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/button" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/buzzer" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/led" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/motor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/rtos" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/sensor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.o: D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.c SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F10X_MD -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Include" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/STM32F10x_StdPeriph_Driver/inc" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/button" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/buzzer" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/led" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/motor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/rtos" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/sensor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-SDK_ROBI_STM32F1_1-2e-0-2e-0-2f-shared-2f-Drivers-2f-CMSIS-2f-Source

clean-SDK_ROBI_STM32F1_1-2e-0-2e-0-2f-shared-2f-Drivers-2f-CMSIS-2f-Source:
	-$(RM) ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.cyclo ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.d ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.o ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/core_cm3.su ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.cyclo ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.d ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.o ./SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Source/system_stm32f10x.su

.PHONY: clean-SDK_ROBI_STM32F1_1-2e-0-2e-0-2f-shared-2f-Drivers-2f-CMSIS-2f-Source

