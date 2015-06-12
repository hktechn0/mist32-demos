#include "io.h"
#include "debug.h"

unsigned int cpos;
dps_sci *dbg_sci;

void debug_init(dps_sci *sci_p)
{
  cpos = 0;
  dbg_sci = sci_p;
}

void debug_putc(char c)
{
  /*
  if(c == '\n') {
    cpos = ((cpos / DISPLAY_CHAR_WIDTH) + 1) * DISPLAY_CHAR_WIDTH;
  }
  else {
    display_putc(gci_nodes[GCI_DISPLAY_NUM].device_area, cpos++, c, 0xfff, 0x000);
  }
  */

  if(dbg_sci) {
    sci_putc(dbg_sci, c);
  }
}

void debug_puts(char *str)
{
  while(*str) {
    debug_putc(*(str++));
  }
}

void debug_put_uint(unsigned int value)
{
  int i;
  unsigned char c;

  debug_putc('0');
  debug_putc('x');

  for(i = 0; i < 8; i++) {
    c = (value & 0xf0000000) >> 28;
    debug_putc((c < 10 ? '0' : 'A' - 10) + c);
    value = (value & 0x0fffffff) << 4;
  }

  debug_putc(' ');
}

void debug_mem(void *p, unsigned int size)
{
  char c, h, l;
  unsigned int i;

  for(i = 0; i < size; i++) {
    if(!(i % 16) && i > 0) {
      debug_putc('\n');
    }

    c = *((char *)p + i);
    h = (c >> 4) & 0xf;
    l = c & 0xf;

    debug_putc((h < 10 ? '0' : 'A' - 10) + h);
    debug_putc((l < 10 ? '0' : 'A' - 10) + l);
    debug_putc(' ');
  }
  debug_putc('\n');
}
