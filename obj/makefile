################################################################################
# MRS Version: 1.9.0
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include LCD/subdir.mk
-include General_Files/drivers/subdir.mk
-include General_Files/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CompositeKM.hex \

SECONDARY_LIST += \
CompositeKM.lst \

SECONDARY_SIZE += \
CompositeKM.siz \


# 所有目标
all: CompositeKM.elf secondary-outputs

# 工具调用
CompositeKM.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -Wextra  -g -T "C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\SRC\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"CompositeKM.map" --specs=nano.specs --specs=nosys.specs -o "CompositeKM.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
CompositeKM.hex: CompositeKM.elf
	@	riscv-none-embed-objcopy -O ihex "CompositeKM.elf"  "CompositeKM.hex"
	@	@
CompositeKM.lst: CompositeKM.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "CompositeKM.elf" > "CompositeKM.lst"
	@	@
CompositeKM.siz: CompositeKM.elf
	@	riscv-none-embed-size --format=berkeley "CompositeKM.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) CompositeKM.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
