################################################################################
# MRS Version: 1.9.0
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../General_Files/drivers/WS2812_PWM_drv.c 

OBJS += \
./General_Files/drivers/WS2812_PWM_drv.o 

C_DEPS += \
./General_Files/drivers/WS2812_PWM_drv.d 


# Each subdirectory must supply rules for building sources it contributes
General_Files/drivers/%.o: ../General_Files/drivers/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -Wextra  -g -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\SRC\Debug" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\SRC\Core" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\User" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\SRC\Peripheral\inc" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\LCD" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\General_Files" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\General_Files\drivers" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

