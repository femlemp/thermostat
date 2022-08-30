PROJECT = thermostat
TARGET = $(PROJECT).elf
MCU = atmega328p
F_CPU = 16000000UL

##translate
СС = avr-gcc 
CFLAGS = -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall --std=c99
SRC_FILES = ./main.c ./twi.c ./drive_seg.c ./adc.c ./timer.c ./usart.c
OBJ_FILES = $(SRC_FILES:.c=.o)
INC_FILES = -MD -MP -MT $(*F).o -MF $(*F).d
COMP_MCU = -mmcu=$(MCU)
OBJ_HEX = avr-objcopy
OBJ_LST = avr-objdump

##program
PROGRAMM = avrdude
PROGRAMMER = usbasp
PROG_MCU = m328p
FUSE_H = 0xD9
FUSE_L = 0xFF
FUSE_EXT = 0xFF
PROG = $(PROGRAMM) -c $(PROGRAMMER) -p $(PROG_MCU)
FLASH_FLAGS = -j .text -j .data -j .fuse -j .lock -j .signature
LDFLAGS = $(COMP_MCU) -Wl,-Map=$(PROJECT).map
SIZE = size 
DUMP_FLAGS = -h -S

all: $(PROJECT).hex $(PROJECT).lst

%.o: %.c
	$(СС) $(COMP_MCU) $(CFLAGS) $(INC_FILES) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(СС) $(LDFLAGS) $^ -o $@

%.hex: $(TARGET)
	$(OBJ_HEX) $(FLASH_FLAGS) -O ihex $< $@
	$(SIZE) $(TARGET)

%.lst: $(TARGET)
	$(OBJ_LST) $(DUMP_FLAGS) $< > $@

.PHONY: all clean flash fuse connect version

clean:
	rm -rf $(OBJ_FILES) $(TARGET) $(PROJECT).elf *.d

flash:
	$(PROG) -U flash:w:$(PROJECT).hex
	##-U eeprom:w:$(PROJECT).eep
fuse:
	$(PROG) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m -U efuse:w:$(FUSE_EXT):m
connect:
	$(PROG)
version:
	($СС) --version > README.md.txt && cat README.md.txt | head -n1 | tail -n1 >> README.md && rm -rf README.md.txt
