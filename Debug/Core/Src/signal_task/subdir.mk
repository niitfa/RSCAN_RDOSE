################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/signal_task/ads1246.c \
../Core/Src/signal_task/signal_task.c 

OBJS += \
./Core/Src/signal_task/ads1246.o \
./Core/Src/signal_task/signal_task.o 

C_DEPS += \
./Core/Src/signal_task/ads1246.d \
./Core/Src/signal_task/signal_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/signal_task/%.o Core/Src/signal_task/%.su Core/Src/signal_task/%.cyclo: ../Core/Src/signal_task/%.c Core/Src/signal_task/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/Src/command_executer -I../Core/Src/display_task -I../Core/Src/slave_uart -I../Core/Src/ssd1306 -I../Core/Src/w5500_task -I../Core/Src/hv_task -I../Core/Src/signal_task -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-signal_task

clean-Core-2f-Src-2f-signal_task:
	-$(RM) ./Core/Src/signal_task/ads1246.cyclo ./Core/Src/signal_task/ads1246.d ./Core/Src/signal_task/ads1246.o ./Core/Src/signal_task/ads1246.su ./Core/Src/signal_task/signal_task.cyclo ./Core/Src/signal_task/signal_task.d ./Core/Src/signal_task/signal_task.o ./Core/Src/signal_task/signal_task.su

.PHONY: clean-Core-2f-Src-2f-signal_task

