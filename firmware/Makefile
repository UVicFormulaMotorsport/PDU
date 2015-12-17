################################################################################
# Project
################################################################################
PROJECT = pdu
TARGET  = atmega2560
F_CPU   = 16000000L


################################################################################
# Compiler Options
################################################################################
OPTIMIZATION = s
STANDARD     = c11


################################################################################
# Programmer Options
################################################################################
PROGRAMMER         = stk500v2
PROGRAMMER_PORT    = /dev/cu.usbmodem*
PROGRAMMER_OPTIONS = -b 115200


################################################################################
# Directories
################################################################################
INCLUDE_DIR   = ./include
SOURCE_DIR    = ./src


################################################################################
# Tools
################################################################################
CC      = avr-gcc
AR      = avr-ar
LD      = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size
AVRDUDE = avrdude
REMOVE  = rm


################################################################################
# Source Files
################################################################################
GCC_SOURCES = $(shell find $(SOURCE_DIR) -type f -name '*.c')

OBJECTS = $(GCC_SOURCES:.c=.o)


################################################################################
# Tool Flags
################################################################################
GCC_FLAGS  = -g -std=$(STANDARD) -mmcu=$(TARGET) -O$(OPTIMIZATION) -DF_CPU=$(F_CPU) -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -Wstrict-prototypes
GCC_FLAGS += $(patsubst %,-I%,$(INCLUDE_DIR)) -I.

LD_FLAGS  = -mmcu=$(TARGET) -DF_CPU=$(F_CPU)
LD_FLAGS += $(patsubst %,-I%,$(INCLUDE_DIR)) -I.

AVRDUDE_FLAGS = -q -q -q -q -F -p $(TARGET) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) $(PROGRAMMER_OPTIONS)


################################################################################
# Targets: Actions
################################################################################
.SUFFIXES: .c .eep .h .hex .o .out .s .S
.PHONY: all disasm hex upload clean

all: $(PROJECT).out

disasm: $(PROJECT).out
	$(OBJDUMP) -D -S $< > $(PROJECT).s

hex: all $(PROJECT).hex $(PROJECT).eep

upload: hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(PROJECT).hex # -U eeprom:w:$(PROJECT).eep

clean:
	find . \( -type f -name '*.o' -o -name '*.s' -o -name '*.out' -o -name '*.hex' \) -exec $(REMOVE) {} \;


################################################################################
# Targets: Output
################################################################################
$(PROJECT).out: $(OBJECTS)
	$(LD) -o $@ $(OBJECTS) $(LD_FLAGS)

%.o: %.c
	$(CC) $(GCC_FLAGS) -c $< -o $@

%.hex: %.out
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	
%.eep: %.out
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 $< $@
