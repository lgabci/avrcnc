AS := avr-as
ASFLAGS := -mmcu=avr4 -mno-skip-bug

CC := avr-gcc
CCFLAGS := -mmcu=atmega8a -Os -S

LD := avr-ld
#LD := /usr/lib/gcc/avr/4.8.1/collect2
LDFLAGS := -m avr4 /usr/lib/gcc/avr/4.8.1/../../../avr/lib/avr4/crtm8a.o -L/usr/lib/gcc/avr/4.8.1/avr4 -L/usr/lib/gcc/avr/4.8.1/../../../avr/lib/avr4 -L/usr/lib/gcc/avr/4.8.1 -L/usr/lib/gcc/avr/4.8.1/../../../avr/lib --start-group -lgcc -lm -lc --end-group

DEV := $(shell ls /dev/ttyACM* 2>/dev/null)
AD := avrdude
ADFLAGS := -p atmega8 -c stk500v2 -P $(DEV)

HFUSE := 0xD9
LFUSE := 0xE4

.DELETE_ON_ERROR :
.SUFFIXES :
.PHONY : flash clean

all : flash

%.d : %.c
	@$(CC) -M $(CCFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

loaderobjs := prog.o
ifneq ($(MAKECMDGOALS),clean)
include $(loaderobjs:.o=.d)
endif

%.s : %.c
	$(CC) $(CCFLAGS) -o $@ $<

%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $<

prog.elf : $(loaderobjs)
	$(LD) $(LDFLAGS) -Map=$(patsubst %.elf,%.map,$@) -o $@ $^
	chmod -x $@

prog.hex : prog.elf
	#avr-objcopy -j .text -j .data -O ihex $^ $@
	avr-objcopy -O ihex $^ $@

flash : prog.hex
	@if [ "$(DEV)" = "" ]; then \
	  echo AVR-Doper device not found >&2; false; \
	fi
	$(AD) $(ADFLAGS) -qq -U flash:v:$^ || $(AD) $(ADFLAGS) -U flash:w:$^
	$(AD) $(ADFLAGS) -qq -U hfuse:v:$(HFUSE):m || $(AD) $(ADFLAGS) -U hfuse:w:$(HFUSE):m
	$(AD) $(ADFLAGS) -qq -U lfuse:v:$(LFUSE):m || $(AD) $(ADFLAGS) -U lfuse:w:$(LFUSE):m

# clean ------------------------------------------------------------------------
clean :
	rm -f *.d *.s *.o *.elf *.bin *.map
