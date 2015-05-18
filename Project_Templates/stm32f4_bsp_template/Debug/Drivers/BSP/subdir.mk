################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/LCD_Touch_Calibration.c \
../Drivers/BSP/fonts.c \
../Drivers/BSP/stm32f4_ask25.c \
../Drivers/BSP/stm32f4_ask25_25aa160a.c \
../Drivers/BSP/stm32f4_ask25_at24c16.c \
../Drivers/BSP/stm32f4_ask25_lcd.c \
../Drivers/BSP/stm32f4_discovery.c \
../Drivers/BSP/stm32f4_discovery_accelerometer.c \
../Drivers/BSP/stm32f4_discovery_camera.c \
../Drivers/BSP/stm32f4_discovery_lcd.c \
../Drivers/BSP/stm32f4_discovery_tsc.c \
../Drivers/BSP/stm32f4_discovery_uart.c \
../Drivers/BSP/stm32f4_discovery_vcp.c 

OBJS += \
./Drivers/BSP/LCD_Touch_Calibration.o \
./Drivers/BSP/fonts.o \
./Drivers/BSP/stm32f4_ask25.o \
./Drivers/BSP/stm32f4_ask25_25aa160a.o \
./Drivers/BSP/stm32f4_ask25_at24c16.o \
./Drivers/BSP/stm32f4_ask25_lcd.o \
./Drivers/BSP/stm32f4_discovery.o \
./Drivers/BSP/stm32f4_discovery_accelerometer.o \
./Drivers/BSP/stm32f4_discovery_camera.o \
./Drivers/BSP/stm32f4_discovery_lcd.o \
./Drivers/BSP/stm32f4_discovery_tsc.o \
./Drivers/BSP/stm32f4_discovery_uart.o \
./Drivers/BSP/stm32f4_discovery_vcp.o 

C_DEPS += \
./Drivers/BSP/LCD_Touch_Calibration.d \
./Drivers/BSP/fonts.d \
./Drivers/BSP/stm32f4_ask25.d \
./Drivers/BSP/stm32f4_ask25_25aa160a.d \
./Drivers/BSP/stm32f4_ask25_at24c16.d \
./Drivers/BSP/stm32f4_ask25_lcd.d \
./Drivers/BSP/stm32f4_discovery.d \
./Drivers/BSP/stm32f4_discovery_accelerometer.d \
./Drivers/BSP/stm32f4_discovery_camera.d \
./Drivers/BSP/stm32f4_discovery_lcd.d \
./Drivers/BSP/stm32f4_discovery_tsc.d \
./Drivers/BSP/stm32f4_discovery_uart.d \
./Drivers/BSP/stm32f4_discovery_vcp.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/%.o: ../Drivers/BSP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F407xx -DUSE_HAL_DRIVER -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_STM32F4_ASK25 -DUSE_STM32F4_UART -DUSE_STM32F4_VCP -DUSE_USB_FS -DUSE_STM32F4_CAMERA -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../semihosting/inc -I../Drivers/BSP -I../Drivers/Components/cs43l22 -I../Drivers/Middlewares/ST/STM32_Audio/Addons/PDM -I../Drivers/Components/lis3dsh -I../Drivers/Components/Common -I../Drivers/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Components/lis302dl -I../Drivers/Components/ov9655 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


