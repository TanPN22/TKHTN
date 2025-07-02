################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/DH/20242\ TK\ He\ thong\ nhung/STM32\ Homework\ Prj/TKHTNhung_BTL_20242/MyLib/MPU9250/MPU9250.c 

OBJS += \
./MPU9250/MPU9250.o 

C_DEPS += \
./MPU9250/MPU9250.d 


# Each subdirectory must supply rules for building sources it contributes
MPU9250/MPU9250.o: D:/DH/20242\ TK\ He\ thong\ nhung/STM32\ Homework\ Prj/TKHTNhung_BTL_20242/MyLib/MPU9250/MPU9250.c MPU9250/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/P2_1_TKHTNhung_BTL_20243/MyLib/MPU9250" -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/P2_1_TKHTNhung_BTL_20243/MyLib/Servo" -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/P2_1_TKHTNhung_BTL_20243/MyLib/HCSR04" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -include"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/P2_1_TKHTNhung_BTL_20243/MyLib/HCSR04/HCSR04.h" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MPU9250

clean-MPU9250:
	-$(RM) ./MPU9250/MPU9250.cyclo ./MPU9250/MPU9250.d ./MPU9250/MPU9250.o ./MPU9250/MPU9250.su

.PHONY: clean-MPU9250

