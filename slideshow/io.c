#include "common.h"

gci_hub_info *gci_hub;
gci_hub_node *gci_hub_nodes;
gci_node *gci_nodes;

inline void *iosr_read(void)
{
  void *iosr;

  __asm__("sriosr %0" : "=r"(iosr));

  return iosr;
}

void gci_setup(void)
{
  char *iosr;
  char *node;
  unsigned int i;
  
  iosr = iosr_read();
  gci_hub = (gci_hub_info *)(iosr + DPS_SIZE);
  gci_hub_nodes = (gci_hub_node *)((char *)gci_hub + GCI_HUB_HEADER_SIZE);
  gci_nodes = malloc(sizeof(gci_node) * gci_hub->total);

  node = (char *)gci_hub + GCI_HUB_SIZE;

  for(i = 0; i < gci_hub->total; i++) {
    gci_nodes[i].node_info = (gci_node_info *)node;
    gci_nodes[i].device_area = node + GCI_NODE_SIZE;
    node += gci_hub_nodes[i].size;
  }
}

inline void display_putc(void *display_io, unsigned int pos, char c,
			 unsigned int forecolor, unsigned int backcolor)
{
  unsigned int *addr;
  unsigned int cdata = ((backcolor & 0xfff) << 20) | (forecolor & 0xfff) << 8 | c;

  addr = (unsigned int *)display_io;
  addr += (pos % 80) + (pos / 80 * 0x100);

  if(pos < 80 * 34) {
    *addr = cdata;
  }
}

inline void display_put(void *display_io, unsigned int x, unsigned int y, unsigned int color)
{
  register unsigned int *addr;

  addr = (unsigned int *)((char *)display_io + DISPLAY_BITMAP_OFFSET);
  addr += x + (y * WIDTH);

  *addr = color;
}
