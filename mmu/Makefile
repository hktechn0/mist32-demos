CC	= mist32-elf-gcc
CFLAGS	= -std=gnu99 -O -Wall -Tmist32-elf-common.ld

LIBDIR	= ../lib
LIBSRC	= $(LIBDIR)/interrupt.c $(LIBDIR)/memory.c # $(LIBDIR)/io.c 
LIBHEAD	= $(LIBDIR)/interrupt.h $(LIBDIR)/memory.h # $(LIBDIR)/io.h 

SRC	= main.c data.s
BIN	= mmu.bin

all:	$(BIN)

clean:
	rm $(BIN) *.elf

.SUFFIXES:	.elf .bin
.elf.bin:
	mist32-elf-objcopy -O binary $< $@

mmu.bin:	mmu.elf

mmu.elf:	$(SRC) $(LIBSRC) $(LIBHEAD)
	$(CC) $(CFLAGS) -I $(LIBDIR) -o $@ $(LIBSRC) $(SRC)
