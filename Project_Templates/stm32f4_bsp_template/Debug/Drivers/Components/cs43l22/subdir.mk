################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Components/cs43l22/cs43l22.c 

OBJS += \
./Drivers/Components/cs43l22/cs43l22.o 

C_DEPS += \
./Drivers/Components/cs43l22/cs43l22.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Components/cs43l22/%.o: ../Drivers/Components/cs43l22/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F407xx -DUSE_HAL_DRIVER -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_STM32F4_ASK25 -DUSE_STM32F4_UART -DUSE_STM32F4_VCP -DUSE_USB_FS -DUSE_STM32F4_CAMERA -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../semihosting/inc -I../Drivers/BSP -I../Drivers/Components/cs43l22 -I../Drivers/Middlewares/ST/STM32_Audio/Addons/PDM -I../Drivers/Components/lis3dsh -I../Drivers/Components/Common -I../Drivers/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Components/lis302dl -I../Drivers/Components/ov9655 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


