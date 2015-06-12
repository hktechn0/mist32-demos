CC	= mist32-elf-gcc
CFLAGS	= -nostdlib -std=gnu99 -O3 -Wall

LDSCRIPT = mist32-elf-firmware.ld

LIBDIR	= ../lib
STDLIBDIR	= ../stdlib
LIBSRC	= $(LIBDIR)/debug.c $(LIBDIR)/io.c $(LIBDIR)/memory.c $(STDLIBDIR)/stdlib.c
LIBHEAD	= $(LIBDIR)/debug.h $(LIBDIR)/io.h $(LIBDIR)/memory.c

SRC	= main.c crc32.c
ELF	= firmware.elf
BIN	= firmware.bin

all:	$(BIN)

clean:
	rm -f $(BIN) $(ELF)

.SUFFIXES:	.elf .bin
.elf.bin:
	mist32-elf-objcopy -O binary $< $@

$(BIN):	$(ELF)

$(ELF):	$(SRC) $(LIBSRC) $(LIBHEAD)
	$(CC) $(CFLAGS) -T $(LDSCRIPT) -I $(LIBDIR) -I $(STDLIBDIR) -o $@ $(LIBDIR)/start.s $(LIBSRC) $(SRC)
