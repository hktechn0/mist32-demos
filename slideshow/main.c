#include "common.h"
#include "debug.h"

extern char *_binary_presentation_img01_start;
extern char *_binary_presentation_img02_start;
extern char *_binary_presentation_img03_start;
extern char *_binary_presentation_img04_start;
extern char *_binary_presentation_img05_start;
extern char *_binary_presentation_img06_start;
extern char *_binary_presentation_img07_start;
extern char *_binary_presentation_img08_start;
extern char *_binary_presentation_img09_start;
extern char *_binary_presentation_img10_start;
extern char *_binary_presentation_img11_start;
extern char *_binary_presentation_img12_start;
extern char *_binary_presentation_img13_start;

volatile unsigned int nimg, prev_nimg;
volatile unsigned int breakcode;
volatile void *_dummy_stack;

void __attribute__((interrupt)) int05_keyboard(void)
{
  volatile unsigned int dummy;
  volatile unsigned int keycode;
  
  dummy = gci_nodes[GCI_KEYBOARD_NUM].node_info->int_factor;
  
  while(1) {
    keycode = *((volatile unsigned int *)gci_nodes[GCI_KEYBOARD_NUM].device_area);
    /* debug_put_uint(keycode); */
    
    if(!(keycode & 0x100)) {
      /* invalid */
      break;
    }
    else if((keycode & 0xff) == 0xf0) {
      /* break code */
      breakcode = 1;
      continue;
    }

    if(breakcode) {
      breakcode = 0;
      
      switch(keycode & 0xff) {
      case 0x16: nimg = 1; break;
      case 0x1e: nimg = 2; break;
      case 0x26: nimg = 3; break;
      case 0x25: nimg = 4; break;
      case 0x2e: nimg = 5; break;
      case 0x36: nimg = 6; break;
      case 0x3d: nimg = 7; break;
      case 0x3e: nimg = 8; break;
      case 0x46: nimg = 9; break;
      case 0x45: nimg = 0; break;
      case 0x6b:
	if(nimg != 0) nimg--; break;
      default: nimg++; break;
      }
    }
  }
}

unsigned int convert_endian(unsigned int i)
{
  return (i << 24) | ((i << 8) & 0x00ff0000) | ((i >> 8) & 0x0000ff00) | ((i >> 24) & 0x000000ff);
}

void image_output(void *display_io, unsigned int *image)
{
  unsigned int x, y, p, p1, p2, *img;

  img = image;

  for(y = 0; y < HEIGHT; y++) {
    for(x = 0; x < WIDTH; x += 2) {
      p = *img;
      p2 = p & 0xffff;
      p1 = (p >> 16) & 0xffff;

      display_put(display_io, x, y, p1);
      display_put(display_io, x + 1, y, p2);

      img++;
    }
  }
}

int start(void)
{
  idt_entry *idt;
  void *display_io;

  unsigned int *images[IMAGE_MAX];

  _dummy_stack = malloc(STACK_SIZE);

  nimg = 0;
  prev_nimg = 999999;
  breakcode = 0;

  images[0] = (unsigned int *)&_binary_presentation_img01_start;
  images[1] = (unsigned int *)&_binary_presentation_img02_start;
  images[2] = (unsigned int *)&_binary_presentation_img03_start;
  images[3] = (unsigned int *)&_binary_presentation_img04_start;
  images[4] = (unsigned int *)&_binary_presentation_img05_start;
  images[5] = (unsigned int *)&_binary_presentation_img06_start;
  images[6] = (unsigned int *)&_binary_presentation_img07_start;
  images[7] = (unsigned int *)&_binary_presentation_img08_start;
  images[8] = (unsigned int *)&_binary_presentation_img09_start;
  images[9] = (unsigned int *)&_binary_presentation_img10_start;
  images[10] = (unsigned int *)&_binary_presentation_img11_start;
  images[11] = (unsigned int *)&_binary_presentation_img12_start;
  images[12] = (unsigned int *)&_binary_presentation_img13_start;

  debug_init();

  idt = idt_setup();

  /* keybord */
  idt_entry_setup(idt, IDT_KEYBOARD_NUM, &int05_keyboard);
  idt_entry_enable(idt, IDT_KEYBOARD_NUM);

  idt_set(idt);
  idt_load();
  gci_setup();
  interrupt_enable();

  display_io = gci_nodes[GCI_DISPLAY_NUM].device_area;

  debug_putc('O');
  debug_putc('K');
  debug_putc('.');

  while(1) {
    if(prev_nimg != nimg) {
      nimg %= IMAGE_MAX;
      prev_nimg = nimg;
      image_output(display_io, (unsigned int *)images[prev_nimg]);
    }
  }
}
