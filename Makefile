TARGET   := lab01-gpio
BUILD    := build

CC       := arm-none-eabi-gcc
SIZE     := arm-none-eabi-size

CFLAGS   := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
            -O2 -Wall -Wextra \
            -ffunction-sections -fdata-sections \
            -Isrc

LDFLAGS  := -Tsrc/stm32f407.ld \
            -Wl,--gc-sections \
            -nostartfiles \
            -specs=nosys.specs

SRCS     := src/main.c \
            src/uart.c \
            src/startup_stm32f407.s

OBJS     := $(patsubst %.c, $(BUILD)/%.o, $(filter %.c, $(SRCS))) \
            $(patsubst %.s, $(BUILD)/%.o, $(filter %.s, $(SRCS)))

.PHONY: all clean

all: $(BUILD)/$(TARGET).elf

$(BUILD)/$(TARGET).elf: $(OBJS)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
