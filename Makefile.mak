PROJECT = thermostat
TARGET = $(PROJECT).elf
MCU = atmega328p
F_CPU = 16000000UL

##translate
CROSS-COMPILE = avr-
СC = $(CROSS-COMPILE)gcc
CFLAGS_MACHIN = -mmcu=$(MCU)
CFLAGS_PREPROC = -DF_CPU=$(F_CPU) -MD
CFLAGS_STD_OPTIONS = --std=c99 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums  
CFLAGS_OPTIMZ = -Os 
CFLAGS_DIAG = 
CFLAGS_WARNINGS = -Wall
CFLAGS_DEBUG = 
CFLAGS = $(CFLAGS_MACHIN) $(CFLAGS_DIAG) $(CFLAGS_DEBUG) $(CFLAGS_PREPROC) $(CFLAGS_STD_OPTIONS) $(CFLAGS_OPTIMZ) $(CFLAGS_WARNINGS)

LDFLAGS = $(CFLAGS_MACHIN)

OBJCOPY = $(CROSS-COMPILE)objcopy
OBJ_FLAGS = -j .text -j .data -j .fuse -j .lock -j .signature 

OBJDUMP = $(CROSS-COMPILE)objdump
DUMPFLAGS = -h -S
DUMPFLAGS_MACHINE = -Pmem-usage

ASSEMBLER = $(CROSS-COMPILE)as
ASFLAGS = -Wa, 

##DIR
SRC = src
SRC_DIR = $(PWD)/$(SRC)/
BUILD = build
BUILD_DIR = $(PWD)/$(BUILD)/
SRC_FILES = timer.c twi.c usart.c adc.c drive_seg.c main.c
OBJS = $(SRC_FILES:.c=.o)
OBJ_FILES := $(addprefix $(BUILD_DIR),$(OBJS))
INC = inc
INC_DIR = $(PWD)/$(INC)/
INC_FILES = -I$(INC_DIR)

##program
PROGRAMM = avrdude
PROGRAMMER = usbasp
PROG_MCU = m328p
FUSE_H = 0xD9
FUSE_L = 0xFF
FUSE_EXT = 0xFF
PROG = $(PROGRAMM) -c $(PROGRAMMER) -p $(PROG_MCU)
SIZE = size
PWD =  $(shell pwd)

##main
all: $(PROJECT).hex

$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(BUILD)
	@$(СC) $(INC_FILES) $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)$(TARGET): $(OBJ_FILES)
	@$(СC) $(LDFLAGS) $^ -o $@
	@$(SIZE) $(BUILD_DIR)$(TARGET)
	@$(OBJDUMP) $(DUMPFLAGS_MACHINE) $(BUILD_DIR)$(TARGET)


%.hex: $(BUILD_DIR)$(TARGET)
	@$(OBJCOPY) $(OBJ_FLAGS) -O ihex $< $@
	@echo HEX_DONE

.PHONY: all clean flash fuse connect version
##command project
clean:
	rm -Rf $(BUILD_DIR)
clean_target:
	rm -Rf *.hex
version:
	($СС) --version > README.md.txt && cat README.md.txt | head -n1 | tail -n1 >> README.md && rm -rf README.md.txt

##command programmer flash file, set fuse etc...
flash:
	$(PROG) -U flash:w:$(PROJECT).hex
	##-U eeprom:w:$(PROJECT).eep
fuse:
	$(PROG) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m -U efuse:w:$(FUSE_EXT):m
connect:
	$(PROG)
