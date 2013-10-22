CC	= mist32-elf-gcc
CFLAGS	= -nostdlib -fomit-frame-pointer -std=gnu99 -O3 -Wall

LIBDIR	= ../lib
STDLIBDIR	= ../stdlib
LIBSRC	= $(LIBDIR)/debug.c $(LIBDIR)/interrupt.c $(LIBDIR)/io.c $(LIBDIR)/memory.c $(STDLIBDIR)/stdlib.c
LIBHEAD	= $(LIBDIR)/debug.h $(LIBDIR)/interrupt.h $(LIBDIR)/io.h $(LIBDIR)/memory.c

SRC	= main.c
ELF	= slideshow_mmc.elf
BIN	= slideshow_mmc.bin

all:	$(BIN)

clean:
	rm $(BIN) $(ELF)

.SUFFIXES:	.elf .bin
.elf.bin:
	mist32-elf-objcopy -O binary $< $@

$(BIN):	$(ELF)

$(ELF):	$(SRC) $(LIBSRC) $(LIBHEAD)
	$(CC) $(CFLAGS) -I $(LIBDIR) -I $(STDLIBDIR) -o $@ $(LIBDIR)/start.s $(LIBSRC) $(SRC)
