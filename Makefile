# output name
BINARY = main

LDSCRIPT 				= stm32f4-discovery.ld

# Using STM32f4 series 
LIBNAME					= opencm3_stm32f4
DEFS						+= -DSTM32F4

# Target-specific flags
FP_FLAGS				?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS			= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)

################################################################################
# OpenOCD specific variables

OOCD						?= openocd
OOCD_INTERFACE	?= stlink-v2
OOCD_TARGET			?= stm32f4x

################################################################################
# Black Magic Probe specific variables
# Set the BMP_PORT to a serial port and then BMP is used for flashing
BMP_PORT	?=

################################################################################

include rules.mk
