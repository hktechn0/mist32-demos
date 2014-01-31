#include <stdlib.h>

#include <memory.h>
#include <io.h>
#include <interrupt.h>
#include <debug.h>

#define IMAGE_MAX 13
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_DEPTH 16
#define IMAGE_SIZE ((IMAGE_DEPTH / 8) * IMAGE_WIDTH * IMAGE_HEIGHT)

volatile unsigned int nimg, prev_nimg;
volatile unsigned int breakcode;

void __attribute__((interrupt)) int05_keyboard(void)
{
  unsigned int scancode;

  while(1) {
    scancode = gci_kmc_scancode(&gci_nodes[GCI_KMC_NUM]);
    /* debug_put_uint(scancode); */

    if(!(scancode & KMC_SCANCODE_VALID)) {
      /* invalid */
      break;
    }
    else if((scancode & 0xff) == KMC_BREAKCODE) {
      breakcode = 1;
      continue;
    }

    if(breakcode) {
      breakcode = 0;

      switch(scancode & 0xff) {
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

  gci_interrupt_ack(&gci_nodes[GCI_KMC_NUM]);
}

void image_output(unsigned int *img)
{
  unsigned int x, y, p1, p2;

  for(y = 0; y < IMAGE_HEIGHT; y++) {
    for(x = 0; x < IMAGE_WIDTH; x++) {
      p2 = *img & 0xffff;
      p1 = (*img >> 16) & 0xffff;

      display_put(gci_nodes[GCI_DISPLAY_NUM].device_area, x++, y, p1);
      display_put(gci_nodes[GCI_DISPLAY_NUM].device_area, x, y, p2);

      img++;
    }
  }
}

int start(void)
{
  idt_entry *idt;
  void *buf;
  unsigned int sector;
  int i;

  nimg = 0;
  prev_nimg = 999999;
  breakcode = 0;

  /* alloc stack */
  malloc(STACK_SIZE);

  /* idt initialize */
  idt = idt_setup();

  /* keybord */
  idt_entry_setup(idt, IDT_GCI_KMC_NUM, &int05_keyboard);
  idt_entry_enable(idt, IDT_GCI_KMC_NUM);

  /* load idt */
  idt_set(idt);
  idt_load();

  /* gci setup */
  gci_setup();

  /* enable interrupt */
  interrupt_enable();

  /* MMC init */
  gci_mmcc_init(&gci_nodes[GCI_MMCC_NUM]);

  debug_init(NULL);
  debug_puts("OK.");
  debug_puts("MEM: ");
  debug_put_uint(dps_mi_memsize());

  buf = malloc(IMAGE_SIZE);

  while(1) {
    if(prev_nimg != nimg) {
      nimg %= IMAGE_MAX;
      prev_nimg = nimg;

      sector = nimg * IMAGE_SIZE / MMCC_SECTOR_SIZE;

      for(i = 0; i < IMAGE_SIZE; i += MMCC_SECTOR_SIZE) {
	gci_mmcc_read(&gci_nodes[GCI_MMCC_NUM], sector++, (char *)buf + i);
      }

      image_output(buf);
    }
  }
}
