################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include semihosting/src/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Inc/subdir.mk
-include Drivers/Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Drivers/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/Components/ov9655/subdir.mk
-include Drivers/Components/lis3dsh/subdir.mk
-include Drivers/Components/lis302dl/subdir.mk
-include Drivers/Components/cs43l22/subdir.mk
-include Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/subdir.mk
-include Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/subdir.mk
-include Drivers/BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
stm32f4_bsp_template.hex \

SECONDARY_SIZE += \
stm32f4_bsp_template.siz \


# All Target
all: stm32f4_bsp_template.elf secondary-outputs

# Tool invocations
stm32f4_bsp_template.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "C:\Edu_Workspace\STM32F4\stm32f4_bsp_template\TrueSTUDIO\stm32f4_bsp_template Configuration\STM32F407VG_FLASH.ld" -Xlinker --gc-sections -Wl,-Map,"stm32f4_bsp_template.map" --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group -o "stm32f4_bsp_template.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

stm32f4_bsp_template.hex: stm32f4_bsp_template.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "stm32f4_bsp_template.elf"  "stm32f4_bsp_template.hex"
	@echo 'Finished building: $@'
	@echo ' '

stm32f4_bsp_template.siz: stm32f4_bsp_template.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley -x "stm32f4_bsp_template.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(S_UPPER_DEPS) stm32f4_bsp_template.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
