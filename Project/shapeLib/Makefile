all: libShape.a shapedemo.elf shapedemo2.elf shapedemo3.elf

CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h 
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as
AR              = msp430-elf-ar

OBJECTS         = shape.o region.o rect.o rect2.o rect3.o rect4.o vec2.o layer.o

libShape.a: $(OBJECTS)
	$(AR) crs $@ $^

$(OBJECTS): shape.h

install: libShape.a
	mkdir -p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

clean:
	rm -f libShape.a *.o *.elf


