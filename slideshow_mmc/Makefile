CC	= mist32-elf-gcc
CFLAGS	= -nostdlib -fomit-frame-pointer -std=gnu99 -O3 -Wall

LIBDIR	= ../lib
LIBSRC	= $(LIBDIR)/start.s $(LIBDIR)/debug.c $(LIBDIR)/common.c $(LIBDIR)/interrupt.c $(LIBDIR)/io.c
LIBHEAD	= $(LIBDIR)/debug.h $(LIBDIR)/common.h $(LIBDIR)/interrupt.h $(LIBDIR)/io.h

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
	$(CC) $(CFLAGS) -I $(LIBDIR) -o $@ $(LIBSRC) $(SRC)
