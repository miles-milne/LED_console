################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Games/Game_2048.cpp \
../Games/Game_Neutron.cpp 

OBJS += \
./Games/Game_2048.o \
./Games/Game_Neutron.o 

CPP_DEPS += \
./Games/Game_2048.d \
./Games/Game_Neutron.d 


# Each subdirectory must supply rules for building sources it contributes
Games/%.o Games/%.su Games/%.cyclo: ../Games/%.cpp Games/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../MyCode -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Games

clean-Games:
	-$(RM) ./Games/Game_2048.cyclo ./Games/Game_2048.d ./Games/Game_2048.o ./Games/Game_2048.su ./Games/Game_Neutron.cyclo ./Games/Game_Neutron.d ./Games/Game_Neutron.o ./Games/Game_Neutron.su

.PHONY: clean-Games

