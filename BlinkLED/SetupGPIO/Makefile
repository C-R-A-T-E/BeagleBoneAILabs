# 

PROJ_NAME = SetupGPIO

CHIP = am57xx
CHIP_REV = am572x_2_0

CFLAGS=-g3 -Wall
LDFLAGS=
OUT_DIR = out

TARGET = $(OUT_DIR)/$(PROJ_NAME)
SOURCES = $(wildcard *.c)
OBJS     =$(patsubst %,$(OUT_DIR)/%,$(SOURCES:.c=.o))

all: $(TARGET) setup

$(TARGET): $(OBJS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJS)

$(OUT_DIR)/%.o: %.c
	mkdir -p $(OUT_DIR)
	gcc $(CFLAGS) -c -o $@ $<

setup: $(TARGET)
	@echo "Setting up /sys/class/gpio/gpio76 (gpio3_12) for use by Method2 and Method3 samples"
	@$(TARGET)

clean:
	rm -rf $(OUT_DIR)
