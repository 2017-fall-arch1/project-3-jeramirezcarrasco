# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:shapemotion.elf

#additional rules for files
BitsInit.o: BitsInit.c BitsInit.h
shapemotion.elf: ${COMMON_OBJECTS} shapemotion.o wdt_handler.o BitsInit.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lShape

load: shapemotion.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
