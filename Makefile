include config.mk

ASM_FILES	= $(wildcard Demo/*.S)

C_FILES 	= $(wildcard Demo/*.c) 								 \
			  $(wildcard Source/*.c)							 \
			  $(wildcard Source/portable/GCC/ARM_Cortex-A15/*.c) \
			  $(wildcard Source/portable/MemMang/heap_3.c)		 \
			  $(wildcard lib/printf.c)							 \
			  $(wildcard drivers/*.c)

OBJS 		:= $(ASM_FILES:.S=.o) $(C_FILES:.c=.o)

BIN			= $(TARGET).bin
LD_SCRIPT	= $(PROJECT).lds.S
OUTPUT 		= $(TARGET).axf
MAP			= $(PROJECT).map

AS			= $(CROSS_COMPILE)as
CC			= $(CROSS_COMPILE)gcc
LD			= $(CROSS_COMPILE)ld
NM			= $(CROSS_COMPILE)nm
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

CFLAGS		= -fPIC -Wall -fno-builtin -fomit-frame-pointer -fno-stack-protector -g
#CFLAGS		= -fPIC
#CFLAGS		= -fomit-frame-pointer -fPIC -fno-stack-protector
LDFLAGS		= -static -nostdlib -nodefaultlibs -nostartfiles
#LDFLAGS		= -nostdlib -nodefaultlibs -nostartfiles

INCLUDES	= -I Demo
INCLUDES	+= -I Demo/Common/include
INCLUDES	+= -I Source/include
INCLUDES	+= -I Source/portable/GCC/ARM_Cortex-A15
INCLUDES	+= -I include $(CFLAGS) 
#LIBS		= /root/work/arm-2014.05/arm-none-eabi/lib
#LIBS		= /usr/lib/arm-none-eabi/lib/libc.a /usr/lib/gcc/arm-none-eabi/4.8.2/libgcc.a

CPPFLAGS	= $(CONFIG_FLAG) $(CFLAGS) $(INCLUDES) -nostartfiles $(LDFLAGS) $(DEBUG_FLAG)
CPPFLAGS	+= -Wall
OCFLAGS 	= -O binary -R .note -R .comment -S
ELF32_LDFLAGS = -Wl,-T,ld-script

#$(OBJS) $(OUTPUT) $(MAP) $(BIN)
all: 
	@$(AS) -mcpu=cortex-a15 Demo/startup.S -o Demo/startup.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Demo/main.c -o Demo/main.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Demo/gtimer.c -o Demo/gtimer.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Demo/gic.c -o Demo/gic.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c drivers/serial.c -o drivers/serial.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c drivers/gpio.c -o drivers/gpio.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c lib/printf.c -o lib/printf.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c lib/vsprintf.c -o lib/vsprintf.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Source/croutine.c -o Source/croutine.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Source/list.c -o Source/list.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Source/queue.c -o Source/queue.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Source/tasks.c -o Source/tasks.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Source/timers.c -o Source/timers.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Source/portable/GCC/ARM_Cortex-A15/port.c -o Source/portable/GCC/ARM_Cortex-A15/port.o
	@$(CC) -mcpu=cortex-a15 $(INCLUDES) -c Source/portable/MemMang/heap_1.c -o Source/portable/MemMang/heap_1.o
	@$(CC) $(LDFLAGS) $(ELF32_LDFLAGS) Demo/main.o Demo/gic.o Demo/gtimer.o Demo/startup.o drivers/serial.o drivers/gpio.o lib/printf.o lib/vsprintf.o Source/croutine.o Source/list.o Source/queue.o Source/tasks.o Source/timers.o Source/portable/GCC/ARM_Cortex-A15/port.o Source/portable/MemMang/heap_1.o -o $(TARGET).elf
	$(OBJCOPY) $(OCFLAGS) $(TARGET).elf $(BIN)
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).dis	
	$(NM) $(TARGET).elf | sort > $(TARGET).map
	mkimage -A arm -a 0xa0000000 -e 0xa0000000 -n FreeRTOS -C none -d $(BIN) freertos_uImage
	ls -lh Demo/main.o Demo/gic.o Demo/gtimer.o Demo/startup.o drivers/serial.o lib/printf.o lib/vsprintf.o Source/croutine.o Source/list.o Source/queue.o Source/tasks.o Source/timers.o Source/portable/GCC/ARM_Cortex-A15/port.o Source/portable/MemMang/heap_1.o
	ls -lh free*
	cp $(BIN) /tftpboot/freeRTOS
	cp freertos_uImage /tftpboot/freertos_uImage

$(MAP): $(OUTPUT)
	$(NM) $< > $@

clean:
	rm -f $(MAP) $(OUTPUT) $(BIN) $(OBJS) $(TARGET).elf $(TARGET).dis freertos_uImage

#$(CC) -Wl,-T,Demo/hello_arndale.ld -nostartfiles -L $(LIBS) -mcpu=cortex-a15 -g -gdwarf-2 -o $@ $(OBJS) -lc -lcs3unhosted 

#$(CC) $(CPPFLAGS) -e Vector_Init -T Demo/generic.ld -o $@ $(OBJS) -L $(LIBS) -lc -lcs3unhosted

$(BIN): $(OUTPUT)
	$(OBJCOPY) -O binary $(OUTPUT) $(BIN)


force: ;

Makefile: ;

.PHONY: all clean config.mk
