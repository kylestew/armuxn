# https://github.com/rschlaikjer/hello-stm32-0-blinky/blob/master/Makefile

#################################
# Target / Hardware Specific
#################################
TARGET 			= blinky
LDSCRIPT 		= stm32f4-discovery.ld
LIBNAME 		= opencm3_stm32f4
DEFS 			+= -DSTM32F4
FP_FLAGS		?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS		= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)


#################################
# OpenOCD variables
#################################
OOCD			?= openocd
OOCD_INTERFACE	?= stlink-v2
OOCD_TARGET		?= stm32f4x


#################################
# Working directories
# Assuming opencm3 sub-repo at /lib/libopencm3
#################################
ROOT		 	= .
SRC_DIR		 	= $(ROOT)/src
BUILD_DIR 	 	= $(ROOT)/build
OPENCM3_DIR	 	= $(ROOT)/lib/libopencm3


#################################
# GNU ARM Embedded Toolchain
#################################
PREFIX 		?= arm-none-eabi
CC 			:= $(PREFIX)-gcc
CXX 		:= $(PREFIX)-g++
LD 			:= $(PREFIX)-gcc
AR			:= $(PREFIX)-ar
AS			:= $(PREFIX)-as
SIZE		:= $(PREFIX)-size
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB			:= $(PREFIX)-gdb
OPT			:= -Os
DEBUG		:= -ggdb3
CSTD		?= -std=c99


#################################
# Flags
#################################
# C
CFLAGS 		+= $(OPT) $(CSTD) $(DEBUG)
CFLAGS 		+= $(ARCH_FLAGS)
CFLAGS 		+= -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS 		+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS 		+= -fno-common -ffunction-sections -fdata-sections

# C++
# TODO

# C & C++ preprocessor common flags
CPPFLAGS 	+= -MD
CPPFLAGS 	+= -Wall -Wundef
CPPFLAGS 	+= $(DEFS)

# Linker
LDFLAGS 	+= --static -nostartfiles
LDFLAGS 	+= -T$(LDSCRIPT)
LDFLAGS 	+= $(ARCH_FLAGS) $(DEBUG)
LDFLAGS		+= -Wl,-Map=$(BUILD_DIR)/$(TARGET).map -Wl,--cref
LDFLAGS		+= -Wl,--gc-sections
ifeq ($(V),99)
TGT_LDFLAGS		+= -Wl,--print-gc-sections
endif

# Libraries
DEFS 		+= -I$(OPENCM3_DIR)/include
LDFLAGS 	+= -L$(OPENCM3_DIR)/lib
LDLIBS 		+= -l$(LIBNAME)
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group


#################################
# Source Files
#################################
SRCS := $(shell find $(SRC_DIR) -name *.cpp -or -name *.c)


#################################
# Object List
#################################
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)


#################################
# Target Output Files
#################################
TARGET_ELF=$(BUILD_DIR)/$(TARGET).elf
TARGET_HEX=$(BUILD_DIR)/$(TARGET).hex


#################################
# Build
#################################

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex --set-start 0x8000000 $< $@

$(TARGET_ELF): $(OBJS) 
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	$(SIZE) $(TARGET_ELF)

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

flash: 
	@printf "  FLASH   $<\n"
	(echo "halt; program $(realpath $(*).elf) verify reset" | nc -4 localhost 4444 2>/dev/null) || \
		$(OOCD) -f interface/$(OOCD_INTERFACE).cfg \
		-f target/$(OOCD_TARGET).cfg \
		-c "program $(*).elf verify reset exit" \
		$(NULL)


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)














include $(OPENCM3_DIR)/mk/genlink-rules.mk
