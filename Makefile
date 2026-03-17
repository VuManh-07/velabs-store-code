# Embedded branch Makefile — safe for: make clean && make -j$(nproc)
TARGET = main
SRCS   = main.s

OBJS = $(SRCS:.s=.o)

LINKER_SCRIPT = stm32.ld

CFLAGS  += -fno-common -Wall -O0 -g3 -mcpu=cortex-m3 -mthumb
LDFLAGS += -nostartfiles -T$(LINKER_SCRIPT)

CROSS_COMPILE = arm-none-eabi-
CC      = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE    = $(CROSS_COMPILE)size

all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.s
	@echo "Building $<"
	$(CC) $(CFLAGS) -c $< -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.lst: %.elf
	$(OBJDUMP) -x -S $< > $@

size: $(TARGET).elf
	$(SIZE) $(TARGET).elf

clean:
	@echo "Cleaning..."
	@rm -f $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).lst $(OBJS)

.PHONY: all clean size
