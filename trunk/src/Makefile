PRG            = main
OBJ            = main.o #uart_command_lib.o corbomite.o
MCU_TARGET     = atmega168
OPTIMIZE       = -Os

#DEFS           =-isystem /usr/avr/include/avr/include
DEFS           =-isystem /usr/avr/include/
#LIBS           =-L/usr/avr/lib -L/usr/avr/lib/avr5
LIBS           =-L/usr/avr/lib/avr5

# You should not have to change anything below here.
CC             = avr-gcc

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall -Werror -pedantic -std=gnu99 $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

doxygenclean:
	
	rm -rf doc/html/*
	rm -rf doc/rtf/*  
	rm -rf doc/latex/*  
	
doxygen:
	doxygen
	cd doc/latex ; make pdf
	cp doc/latex/refman.pdf doc/out/
	cp doc/rtf/refman.rtf doc/out/

load: $(PRG).hex
	sudo avrdude -p m168 -P usb -c avrispmkII -Uflash:w:$(PRG).hex -B 4

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

program: all load
# Rules for building the .text rom images
text: hex bin srec
	avr-size $(PRG).elf

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images
eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@


