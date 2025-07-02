################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/LoRa_lib/lora.c 

OBJS += \
./Core/LoRa_lib/lora.o 

C_DEPS += \
./Core/LoRa_lib/lora.d 


# Each subdirectory must supply rules for building sources it contributes
Core/LoRa_lib/%.o Core/LoRa_lib/%.su Core/LoRa_lib/%.cyclo: ../Core/LoRa_lib/%.c Core/LoRa_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/MCB20242_Provision_WSN2/Core/DHT22" -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/MCB20242_Provision_WSN2/Core/LIBDHT11" -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/MCB20242_Provision_WSN2/Core/LoRa_lib" -IC:/Users/MSI/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/MSI/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/MSI/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/MSI/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-LoRa_lib

clean-Core-2f-LoRa_lib:
	-$(RM) ./Core/LoRa_lib/lora.cyclo ./Core/LoRa_lib/lora.d ./Core/LoRa_lib/lora.o ./Core/LoRa_lib/lora.su

.PHONY: clean-Core-2f-LoRa_lib

