PROJECT = Project
BUILD_DIR = bin

OPENCM3_DIR = ./libopencm3

#commands:
#make to compile and flash through openocd
#make all
#make noflash to compile but not flash

#add all *.c files part of the firmware here
CFILES = main.c
CFILES += queue.c
CFILES += printf.c
CFILES += debug.c

#V=1 for verbose output, V=0 for reduced verbosity
V=1

# TODO - you will need to edit these four lines!
OOCD = ~/opt/openocd-stm32/bin/openocd
#path to openocd binary
DEVICE=stm32g474ceu6
#full chip part number
OOCD_TARGET = stm32g4x
#target/<this>.cfg filename for openocd target
OOCD_INTERFACE = cmsis-dap
#interface/<this>.cfg filename for openocd interface
#st-link is stlink, wch link-e in ARM mode is cmsis-dap

# You shouldn't have to edit anything below here.

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
