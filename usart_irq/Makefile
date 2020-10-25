PROJECT = usart_irq
BUILD_DIR = bin

SHARED_DIR = ../libraries
CFILES = main.c
CFILES += usart.c fifo.c
#AFILES += api-asm.S

# TODO - you will need to edit these two lines!
DEVICE=stm32f103c8
#OOCD_FILE = board/stm32f4discovery.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk


# Black Magic Probe:

GDB=/usr/bin/arm-none-eabi-gdb
BMP_DEVICE=/dev/ttyACM0

bmp: $(PROJECT).elf
	$(GDB) $(PROJECT).elf \
		-ex 'set confirm off' \
		-ex 'target extended-remote $(BMP_DEVICE)' \
		-ex 'monitor swdp_scan' \
		-ex 'attach 1'

