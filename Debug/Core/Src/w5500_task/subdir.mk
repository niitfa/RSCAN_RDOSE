################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/w5500_task/socket.c \
../Core/Src/w5500_task/tcp_server_rx.c \
../Core/Src/w5500_task/tcp_server_tx.c \
../Core/Src/w5500_task/w5500.c \
../Core/Src/w5500_task/w5500_task.c \
../Core/Src/w5500_task/wizchip_conf.c \
../Core/Src/w5500_task/wiznet_api.c 

OBJS += \
./Core/Src/w5500_task/socket.o \
./Core/Src/w5500_task/tcp_server_rx.o \
./Core/Src/w5500_task/tcp_server_tx.o \
./Core/Src/w5500_task/w5500.o \
./Core/Src/w5500_task/w5500_task.o \
./Core/Src/w5500_task/wizchip_conf.o \
./Core/Src/w5500_task/wiznet_api.o 

C_DEPS += \
./Core/Src/w5500_task/socket.d \
./Core/Src/w5500_task/tcp_server_rx.d \
./Core/Src/w5500_task/tcp_server_tx.d \
./Core/Src/w5500_task/w5500.d \
./Core/Src/w5500_task/w5500_task.d \
./Core/Src/w5500_task/wizchip_conf.d \
./Core/Src/w5500_task/wiznet_api.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/w5500_task/%.o Core/Src/w5500_task/%.su Core/Src/w5500_task/%.cyclo: ../Core/Src/w5500_task/%.c Core/Src/w5500_task/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/Src/command_executer -I../Core/Src/display_task -I../Core/Src/slave_uart -I../Core/Src/ssd1306 -I../Core/Src/w5500_task -I../Core/Src/hv_task -I../Core/Src/signal_task -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-w5500_task

clean-Core-2f-Src-2f-w5500_task:
	-$(RM) ./Core/Src/w5500_task/socket.cyclo ./Core/Src/w5500_task/socket.d ./Core/Src/w5500_task/socket.o ./Core/Src/w5500_task/socket.su ./Core/Src/w5500_task/tcp_server_rx.cyclo ./Core/Src/w5500_task/tcp_server_rx.d ./Core/Src/w5500_task/tcp_server_rx.o ./Core/Src/w5500_task/tcp_server_rx.su ./Core/Src/w5500_task/tcp_server_tx.cyclo ./Core/Src/w5500_task/tcp_server_tx.d ./Core/Src/w5500_task/tcp_server_tx.o ./Core/Src/w5500_task/tcp_server_tx.su ./Core/Src/w5500_task/w5500.cyclo ./Core/Src/w5500_task/w5500.d ./Core/Src/w5500_task/w5500.o ./Core/Src/w5500_task/w5500.su ./Core/Src/w5500_task/w5500_task.cyclo ./Core/Src/w5500_task/w5500_task.d ./Core/Src/w5500_task/w5500_task.o ./Core/Src/w5500_task/w5500_task.su ./Core/Src/w5500_task/wizchip_conf.cyclo ./Core/Src/w5500_task/wizchip_conf.d ./Core/Src/w5500_task/wizchip_conf.o ./Core/Src/w5500_task/wizchip_conf.su ./Core/Src/w5500_task/wiznet_api.cyclo ./Core/Src/w5500_task/wiznet_api.d ./Core/Src/w5500_task/wiznet_api.o ./Core/Src/w5500_task/wiznet_api.su

.PHONY: clean-Core-2f-Src-2f-w5500_task

