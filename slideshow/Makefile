CC	= mist32-elf-gcc
CFLAGS	= -nostdlib -fomit-frame-pointer

SRC	= start.s debug.c common.c interrupt.c io.c main.c
BIN	= slideshow.O0.bin slideshow.O1.bin slideshow.O2.bin slideshow.O3.bin

IMGDIR	= bitmap/presentation
IMGOBJS	= $(IMGDIR)/img01.o $(IMGDIR)/img02.o $(IMGDIR)/img03.o $(IMGDIR)/img04.o \
	$(IMGDIR)/img05.o $(IMGDIR)/img06.o $(IMGDIR)/img07.o $(IMGDIR)/img08.o \
	$(IMGDIR)/img09.o $(IMGDIR)/img10.o $(IMGDIR)/img11.o $(IMGDIR)/img12.o \
	$(IMGDIR)/img13.o

all:	$(BIN)

clean:
	rm $(BIN) *.out

.SUFFIXES:	.out .bin
.out.bin:
	mist32-elf-objcopy -O binary $< $@

slideshow.O0.bin:	slideshow.O0.out
slideshow.O1.bin:	slideshow.O1.out
slideshow.O2.bin:	slideshow.O2.out
slideshow.O3.bin:	slideshow.O3.out

slideshow.O0.out:	$(SRC) $(IMGOBJS)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(IMGOBJS)
slideshow.O1.out:	$(SRC) $(IMGOBJS)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(IMGOBJS)
slideshow.O2.out:	$(SRC) $(IMGOBJS)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(IMGOBJS)
slideshow.O3.out:	$(SRC) $(IMGOBJS)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(IMGOBJS)
