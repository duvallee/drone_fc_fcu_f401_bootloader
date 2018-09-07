###############################################################################
#
#  	    File        : Makefile
#
#       Abstract    : Example Makefile for a C Project
#
#       Environment : Atollic TrueSTUDIO(R)
#
###############################################################################
SHELL						= cmd

# -----------------------------------------------------------------------------
DEBUG 					= 1
TARGET					= fcu_f401_bootloader

# -----------------------------------------------------------------------------
USB_BULK             = 1
USB_CDC              = 2
USB_DEVICE           = USB_BULK

# -----------------------------------------------------------------------------
# System configuration
CC 						= arm-atollic-eabi-gcc
OBJCOPY 					= arm-atollic-eabi-objcopy
SIZE    					= arm-atollic-eabi-size

RM							= rm -rf

# Assembler, Compiler and Linker flags and linker script settings
LINK_SCRIPT				= "STM32F401CCUx_FLASH.ld"

# FLAGS
CPU						:= -mcpu=cortex-m4
FPU						:= -mfpu=fpv4-sp-d16
FLOAT-ABI				:= -mfloat-abi=hard

MCU						:= -mthumb $(CPU)  $(FLOAT-ABI) $(FPU)
OPT						:= -Os

# Assembler flags
AS_INCLUDE				:=
AFLAGS					:= $(MCU) $(AS_INCLUDE) $(OPT) -Wa,--no-warn -x assembler-with-cpp -specs=nano.specs
ifeq ($(DEBUG), 1)
AFLAGS					+= -g
endif

# C flags
C_INCLUDE				:= -Iinc
C_INCLUDE				+= -Isrc/common/inc
ifeq ($(USB_DEVICE), $(USB_BULK))
C_INCLUDE				+= -Isrc/usb/bulk/inc
endif
ifeq ($(USB_DEVICE), $(USB_CDC))
C_INCLUDE				+= -Isrc/usb/cdc/inc
endif
C_INCLUDE				+= -IDrivers/STM32F4xx_HAL_Driver/Inc 
C_INCLUDE				+= -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy
C_INCLUDE				+= -IDrivers/CMSIS/Device/ST/STM32F4xx/Include
C_INCLUDE				+= -IDrivers/CMSIS/Include

C_DEFS					:= -D__weak=__attribute__((weak))
C_DEFS					+= -D__packed=__attribute__((__packed__))
C_DEFS					+= -DUSE_HAL_DRIVER
C_DEFS					+= -DSTM32F401xC
ifeq ($(USB_DEVICE), $(USB_BULK))
C_DEFS					+= -DSTM32F401_USB_BULK_DEVICE
endif
ifeq ($(USB_DEVICE), $(USB_CDC))
C_DEFS					+= -DSTM32F401_USB_CDC_DEVICE
endif

CFLAGS					:=  $(MCU) $(C_INCLUDE) $(C_DEFS) $(OPT) -ffunction-sections -fdata-sections -fstack-usage -Wall -specs=nano.specs
ifeq ($(DEBUG), 1)
CFLAGS 					+= -g -gdwarf-2
endif

# Linker flags
LIBS 						= -lc -lm -lnosys 
LIBDIR 					= 

LFLAGS					:= $(MCU) $(MCU) -specs=nano.specs -T$(LINK_SCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BIN_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# Define output directory
OBJECT_DIR 				= Debug
BIN_DIR 					= $(OBJECT_DIR)

# Define sources and objects

# --------------------------------------------------------------------
# System Source
SYSTEM_SOURCE			:= ./src/system_stm32f4xx.c
SYSTEM_SOURCE			+= ./src/stm32f4xx_it.c
SYSTEM_SOURCE			+= ./src/stm32f4xx_hal_msp.c

# HAL Library Source
HAL_LIBRARY_SOURCE 	:= ./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
HAL_LIBRARY_SOURCE 	+= ./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
HAL_LIBRARY_SOURCE 	+= ./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
HAL_LIBRARY_SOURCE 	+= ./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
HAL_LIBRARY_SOURCE 	+= ./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
HAL_LIBRARY_SOURCE 	+= ./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
HAL_LIBRARY_SOURCE 	+= ./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c

# User Source
USER_SOURCE				:= ./src/main.c
USER_SOURCE				+= ./src/common/src/printf.c
USER_SOURCE				+= ./src/common/src/scheduler.c
USER_SOURCE				+= ./src/common/src/uart_debug.c
USER_SOURCE				+= ./src/common/src/battery_gauge.c
USER_SOURCE				+= ./src/common/src/led.c
ifeq ($(USB_DEVICE), $(USB_BULK))
USER_SOURCE				+= ./src/usb/bulk/usb_bulk.c
USER_SOURCE				+= ./src/usb/bulk/usb_bulk_device.c
USER_SOURCE				+= ./src/usb/bulk/usbd_bulk_desc.c
USER_SOURCE				+= ./src/usb/bulk/usbd_bulk_if.c
USER_SOURCE				+= ./src/usb/bulk/usbd_conf.c
endif
ifeq ($(USB_DEVICE), $(USB_CDC))
USER_SOURCE				+=
endif


SRC 						:= $(SYSTEM_SOURCE) $(HAL_LIBRARY_SOURCE) $(USER_SOURCE)

# --------------------------------------------------------------------
# ASM Source
SRCSASM 					:= ./startup/startup_stm32f401xc.s

OBJS 						:= $(SRC:%.c=$(OBJECT_DIR)/%.o) $(SRCSASM:%.s=$(OBJECT_DIR)/%.o)
OBJS 						:= $(OBJS:%.S=$(OBJECT_DIR)/%.o)  

###############
# Build project
# Major targets
###############
all: buildelf

buildelf: $(OBJS) 
	$(CC) -o "$(BIN_DIR)/bootloader.elf" $(OBJS) $(LFLAGS)
	$(OBJCOPY) -O ihex "$(BIN_DIR)/bootloader.elf"   "$(BIN_DIR)/bootloader.hex"
	$(OBJCOPY) -O binary "$(BIN_DIR)/bootloader.elf"   "$(BIN_DIR)/bootloader.bin"
	$(SIZE) "$(BIN_DIR)/bootloader.elf"

clean:
	$(RM) $(OBJS) "$(BIN_DIR)/fcu_f401_bootloader.elf" "$(BIN_DIR)/fcu_f401_bootloader.map"


##################
# Implicit targets
##################
$(OBJECT_DIR)/%.o: %.c
	@mkdir $(subst /,\,$(dir $@)) 2> NUL || echo off
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJECT_DIR)/%.o: %.s
	@mkdir $(subst /,\,$(dir $@)) 2> NUL || echo off
	$(CC) -c $(AFLAGS) $< -o $@
	
$(OBJECT_DIR)/%.o: %.S
	@mkdir $(subst /,\,$(dir $@)) 2> NUL || echo off
	$(CC) -c $(AFLAGS) $< -o $@
