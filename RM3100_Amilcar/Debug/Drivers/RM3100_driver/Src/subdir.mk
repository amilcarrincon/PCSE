################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/RM3100_driver/Src/port.c \
../Drivers/RM3100_driver/Src/rm3100.c 

OBJS += \
./Drivers/RM3100_driver/Src/port.o \
./Drivers/RM3100_driver/Src/rm3100.o 

C_DEPS += \
./Drivers/RM3100_driver/Src/port.d \
./Drivers/RM3100_driver/Src/rm3100.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/RM3100_driver/Src/%.o Drivers/RM3100_driver/Src/%.su: ../Drivers/RM3100_driver/Src/%.c Drivers/RM3100_driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/ESPECIALIZACION SYS EMB/PROTOCOLOS DE COMUNICACION/RM3100_Amilcar/RM3100_Amilcar/Drivers/RM3100_driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-RM3100_driver-2f-Src

clean-Drivers-2f-RM3100_driver-2f-Src:
	-$(RM) ./Drivers/RM3100_driver/Src/port.d ./Drivers/RM3100_driver/Src/port.o ./Drivers/RM3100_driver/Src/port.su ./Drivers/RM3100_driver/Src/rm3100.d ./Drivers/RM3100_driver/Src/rm3100.o ./Drivers/RM3100_driver/Src/rm3100.su

.PHONY: clean-Drivers-2f-RM3100_driver-2f-Src

