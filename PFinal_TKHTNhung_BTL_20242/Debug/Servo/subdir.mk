################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/DH/20242\ TK\ He\ thong\ nhung/STM32\ Homework\ Prj/TKHTNhung_BTL_20242/MyLib/Servo/Servo.c 

OBJS += \
./Servo/Servo.o 

C_DEPS += \
./Servo/Servo.d 


# Each subdirectory must supply rules for building sources it contributes
Servo/Servo.o: D:/DH/20242\ TK\ He\ thong\ nhung/STM32\ Homework\ Prj/TKHTNhung_BTL_20242/MyLib/Servo/Servo.c Servo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/PFinal_TKHTNhung_BTL_20242/MyLib/MPU9250" -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/PFinal_TKHTNhung_BTL_20242/MyLib/Servo" -I"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/PFinal_TKHTNhung_BTL_20242/MyLib/HCSR04" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -include"D:/DH/20242 TK He thong nhung/STM32 Homework Prj/PFinal_TKHTNhung_BTL_20242/MyLib/HCSR04/HCSR04.h" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Servo

clean-Servo:
	-$(RM) ./Servo/Servo.cyclo ./Servo/Servo.d ./Servo/Servo.o ./Servo/Servo.su

.PHONY: clean-Servo

