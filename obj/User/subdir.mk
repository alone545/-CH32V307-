################################################################################
# MRS Version: 1.9.0
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ADC.c \
../User/ch32v30x_it.c \
../User/ch32v30x_usbfs_device.c \
../User/main.c \
../User/system_ch32v30x.c \
../User/usbd_compostie_km.c \
../User/usbd_desc.c 

OBJS += \
./User/ADC.o \
./User/ch32v30x_it.o \
./User/ch32v30x_usbfs_device.o \
./User/main.o \
./User/system_ch32v30x.o \
./User/usbd_compostie_km.o \
./User/usbd_desc.o 

C_DEPS += \
./User/ADC.d \
./User/ch32v30x_it.d \
./User/ch32v30x_usbfs_device.d \
./User/main.d \
./User/system_ch32v30x.d \
./User/usbd_compostie_km.d \
./User/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -Wextra  -g -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\SRC\Debug" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\SRC\Core" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\User" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\SRC\Peripheral\inc" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\LCD" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\General_Files" -I"C:\Users\Lenovo\Desktop\CH32V307EVT\EVT\EXAM\USB\USBFS\DEVICE\CompositeKM (2)\General_Files\drivers" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

