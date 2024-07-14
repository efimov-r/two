###############################################################################
# Makefile for the project two
###############################################################################

## General Flags
PROJECT = two
MCU = atmega32
TARGET = two.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                 -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=two.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main.o lcd_low.o lcd_hight.o adc.o tempure_sensor.o task_volt.o task_external_tempure.o task_oil_tempure.o task_service.o service_function.o task_gazoline.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) two.hex two.eep two.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_low.o: ../lcd_low.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_hight.o: ../lcd_hight.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tempure_sensor.o: ../tempure_sensor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

task_volt.o: ../task_volt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

task_external_tempure.o: ../task_external_tempure.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

task_oil_tempure.o: ../task_oil_tempure.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

task_service.o: ../task_service.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

service_function.o: ../service_function.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

task_gazoline.o: ../task_gazoline.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) two.elf dep/* two.hex two.eep two.lss two.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

