TARGET = main
SRCS = main.s

OBJS =  $(addsuffix .o, $(basename $(SRCS)))

LINKER_SCRIPT = stm32.ld

CFLAGS += -fno-common -Wall -O0 -g3 -mcpu=cortex-m3 -mthumb
LDFLAGS += -nostartfiles -T$(LINKER_SCRIPT)

CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

all: clean $(SRCS) build size
	@echo "Successfully finished..."

build: $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst

$(TARGET).elf: $(OBJS)
	@$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o: %.s
	@echo "Building" $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.hex: %.elf
	@$(OBJCOPY) -O ihex $< $@

%.bin: %.elf
	@$(OBJCOPY) -O binary $< $@

%.lst: %.elf
	@$(OBJDUMP) -x -S $(TARGET).elf > $@

size: $(TARGET).elf
	@$(SIZE) $(TARGET).elf

clean:
	@echo "Cleaning..."
	@rm -f $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).lst $(TARGET).o

.PHONY: all build size clean

