################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/config.c \
../Core/Src/dma.c \
../Core/Src/fdcan.c \
../Core/Src/flash.c \
../Core/Src/flash_doubleword.c \
../Core/Src/flash_pedal_lut.c \
../Core/Src/functions.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/pedal_map.c \
../Core/Src/sensors.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/tim.c \
../Core/Src/transfer_functions.c \
../Core/Src/virtual_sensors.c 

OBJS += \
./Core/Src/adc.o \
./Core/Src/config.o \
./Core/Src/dma.o \
./Core/Src/fdcan.o \
./Core/Src/flash.o \
./Core/Src/flash_doubleword.o \
./Core/Src/flash_pedal_lut.o \
./Core/Src/functions.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/pedal_map.o \
./Core/Src/sensors.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/tim.o \
./Core/Src/transfer_functions.o \
./Core/Src/virtual_sensors.o 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/config.d \
./Core/Src/dma.d \
./Core/Src/fdcan.d \
./Core/Src/flash.d \
./Core/Src/flash_doubleword.d \
./Core/Src/flash_pedal_lut.d \
./Core/Src/functions.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/pedal_map.d \
./Core/Src/sensors.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/tim.d \
./Core/Src/transfer_functions.d \
./Core/Src/virtual_sensors.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/config.cyclo ./Core/Src/config.d ./Core/Src/config.o ./Core/Src/config.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/fdcan.cyclo ./Core/Src/fdcan.d ./Core/Src/fdcan.o ./Core/Src/fdcan.su ./Core/Src/flash.cyclo ./Core/Src/flash.d ./Core/Src/flash.o ./Core/Src/flash.su ./Core/Src/flash_doubleword.cyclo ./Core/Src/flash_doubleword.d ./Core/Src/flash_doubleword.o ./Core/Src/flash_doubleword.su ./Core/Src/flash_pedal_lut.cyclo ./Core/Src/flash_pedal_lut.d ./Core/Src/flash_pedal_lut.o ./Core/Src/flash_pedal_lut.su ./Core/Src/functions.cyclo ./Core/Src/functions.d ./Core/Src/functions.o ./Core/Src/functions.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/pedal_map.cyclo ./Core/Src/pedal_map.d ./Core/Src/pedal_map.o ./Core/Src/pedal_map.su ./Core/Src/sensors.cyclo ./Core/Src/sensors.d ./Core/Src/sensors.o ./Core/Src/sensors.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/transfer_functions.cyclo ./Core/Src/transfer_functions.d ./Core/Src/transfer_functions.o ./Core/Src/transfer_functions.su ./Core/Src/virtual_sensors.cyclo ./Core/Src/virtual_sensors.d ./Core/Src/virtual_sensors.o ./Core/Src/virtual_sensors.su

.PHONY: clean-Core-2f-Src

