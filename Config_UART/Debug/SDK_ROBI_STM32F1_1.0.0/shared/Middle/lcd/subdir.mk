################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.c \
D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.c 

OBJS += \
./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.o \
./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.o 

C_DEPS += \
./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.d \
./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.o: D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.c SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F10X_MD -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Include" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/STM32F10x_StdPeriph_Driver/inc" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/button" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/buzzer" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/led" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/motor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/rtos" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/sensor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.o: D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.c SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F10X_MD -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/CMSIS/Include" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Drivers/STM32F10x_StdPeriph_Driver/inc" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/button" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/buzzer" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/led" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/motor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/rtos" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Middle/sensor" -I"D:/DH/Lumi/LabROBI/Config_UART/SDK_ROBI_STM32F1_1.0.0/shared/Utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-SDK_ROBI_STM32F1_1-2e-0-2e-0-2f-shared-2f-Middle-2f-lcd

clean-SDK_ROBI_STM32F1_1-2e-0-2e-0-2f-shared-2f-Middle-2f-lcd:
	-$(RM) ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.cyclo ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.d ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.o ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/GUI.su ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.cyclo ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.d ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.o ./SDK_ROBI_STM32F1_1.0.0/shared/Middle/lcd/lcd.su

.PHONY: clean-SDK_ROBI_STM32F1_1-2e-0-2e-0-2f-shared-2f-Middle-2f-lcd

