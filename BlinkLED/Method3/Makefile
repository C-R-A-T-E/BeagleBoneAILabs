# Makefile hacked up from TI, Cloud9 & Exploring BeagleBone
#
# Make BlinkLED Method 3
#

PROJ_NAME = BlinkLED

CHIP = am57xx
CHIP_REV = am572x_2_0

PRU_CGT =/usr/lib/ti/pru-software-support-package
LIBS =
INCLUDE = --include_path=$(PRU_CGT)/include --include_path=$(PRU_CGT)/include/$(CHIP_REV)
STACK_SIZE = 0x100
HEAP_SIZE = 0x100
OUT_DIR = out

LINKER_COMMAND_FILE=./$(CHIP)_pru.cmd

PRU1_0 = /sys/class/remoteproc/remoteproc4
PRU1_1 = /sys/class/remoteproc/remoteproc5
PRU2_0 = /sys/class/remoteproc/remoteproc6
PRU2_1 = /sys/class/remoteproc/remoteproc7

CFLAGS = -v3 -O2 --display_error_number --endian=little --hardware_mac=on --obj_directory=$(OUT_DIR) --pp_directory=$(OUT_DIR) -ppd -ppa
LFLAGS = --reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE)

TARGET = $(OUT_DIR)/$(PROJ_NAME)
FIRMWARE = am57xx-pru1_0-blinkled-fw

MAP = $(OUT_DIR)/$(PROJ_NAME).map
SOURCES = $(wildcard *.c)
OBJS    = $(patsubst %,$(OUT_DIR)/%,$(SOURCES:.c=.o))

all: $(TARGET)

$(TARGET): $(OBJS) $(LINKER_COMMAND_FILE)
	/usr/bin/clpru $(CFLAGS) -z -i$(PRU_CGT)/lib -i$(PRU_CGT)/include $(LFLAGS) -o $(TARGET) $(OBJS) -m$(MAP) $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS)

$(OUT_DIR)/%.o: %.c
	@mkdir -p $(OUT_DIR)
	/usr/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<

run: $(TARGET) stop
	@echo "Installing $(TARGET) as frimwareStarting $(PRU1_0)"
	@echo "Must run as privileged user, using sudo"
	@sudo cp $(TARGET) /lib/firmware/$(FIRMWARE)
	@echo $(FIRMWARE) > $(PRU1_0)/firmware
	@echo "Starting $(PRU1_0)"
	@echo start > $(PRU1_0)/state


stop: 
	@PRU1_0_STATE=`cat $(PRU1_0)/state`; \
	if [ $$PRU1_0_STATE = "running" ]; then \
		echo "Stopping $(PRU1_0)"; \
		echo stop > $(PRU1_0)/state; \
	fi


.PHONY: all clean

# Remove the $(OUT_DIR) directory
clean:
	rm -rf $(OUT_DIR)

# Includes the dependencies that the compiler creates (-ppd and -ppa flags)
-include $(OBJS:%.o=%.pp)
