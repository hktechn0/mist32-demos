#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "io.h"

gci_hub_info *gci_hub;
gci_hub_node *gci_hub_nodes;
gci_node *gci_nodes;

/* Memory Information */
unsigned int dps_mi_memsize(void)
{
  return *(unsigned int *)((char *)iosr_read() + DPS_MIMSR);
}

/* Timer */
dps_utim64 *dps_utim64_init(int id)
{
  char *iosr = iosr_read();

  if(id == 0) {
    return (dps_utim64 *)(iosr + DPS_UTIM64A);
  }
  else if(id == 1) {
    return (dps_utim64 *)(iosr + DPS_UTIM64B);
  }
  else {
    return NULL;
  }
}

void dps_utim64_enable(dps_utim64 *utim64)
{
  utim64->mcfg |= UTIM64MCFG_ENA;
}

void dps_utim64_disable(dps_utim64 *utim64)
{
  utim64->mcfg &= ~UTIM64MCFG_ENA;
}

/* Serial */
dps_sci *dps_sci_init(void)
{
  char *iosr = iosr_read();
  return (dps_sci *)(iosr + DPS_SCI);
}

void dps_sci_setup(dps_sci *sci, bool t_en, bool r_en,
		   unsigned int baud, unsigned int t_int, unsigned int r_int)
{
  unsigned int cfg;

  /* baud rate */
  cfg = (baud & 0xf) << SCICFG_BDR_OFFSET;

  /* enable interrupt */
  cfg |= (t_int & 0xf) << SCICFG_TIRE_OFFSET | (r_int & 0xf) << SCICFG_RIRE_OFFSET;

  /* enable transmit and receive */
  cfg |= (t_en ? SCICFG_TEN : 0) | (r_en ? SCICFG_REN : 0);

  sci->cfg = cfg;
}

void dps_sci_clear(dps_sci *sci, bool t_buf, bool r_buf)
{
  /* clear device buffer */
  sci->cfg |= (t_buf ? SCICFG_TCLR : 0) | (r_buf ? SCICFG_RCLR : 0);
}

int sci_putc(dps_sci *sci, int c)
{
  return (sci->txd = (unsigned char)c);
}

int sci_getc(dps_sci *sci)
{
  unsigned int c;
  c = sci->rxd;

  if(c & 0x80000000) {
    return (unsigned char)(c & 0xff);
  }

  return EOF;
}

/* GCI */
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

/* GCI - Display */
void display_putc(void *display_io, unsigned int pos,
		  char c, unsigned int forecolor, unsigned int backcolor)
{
  unsigned int *addr;
  unsigned int cdata = ((backcolor & 0xfff) << 20) | (forecolor & 0xfff) << 8 | c;

  addr = (unsigned int *)display_io;
  addr += (pos % DISPLAY_CHAR_WIDTH) + (pos / DISPLAY_CHAR_WIDTH * 0x100);

  if(pos < DISPLAY_CHAR_WIDTH * DISPLAY_CHAR_HEIGHT && c >= 0x20) {
    *addr = cdata;
  }
}

void display_put(void *display_io, unsigned int x, unsigned int y, unsigned int color)
{
  unsigned int *addr;
  
  addr = (unsigned int *)((char *)display_io + DISPLAY_BITMAP_OFFSET);
  addr += x + (y * DISPLAY_WIDTH);

  *addr = color;
}

/* GCI - MMCC */
void gci_mmcc_init(gci_node *node)
{
  gci_mmcc *mmcc;

  mmcc = node->device_area;
  mmcc->init_command = MMCC_INITIAL_COMMAND;
}

inline void *gci_mmcc_read_req(gci_node *node, unsigned int sector)
{
  gci_mmcc *mmcc;

  mmcc = node->device_area;

  /* LBA to byte address */
  mmcc->sector_read = sector << 9;

  return OFFSET(mmcc, MMCC_BUFFER_OFFSET);
}

void *gci_mmcc_read(gci_node *node, unsigned int sector, void *buf)
{
  void *hwbuf;

  hwbuf = gci_mmcc_read_req(node, sector);
  memcpy(buf, hwbuf, MMCC_BUFFER_SIZE);

  return buf;
}

void *gci_mmcc_read_bswap32(gci_node *node, unsigned int sector, void *buf)
{
  void *hwbuf;

  hwbuf = gci_mmcc_read_req(node, sector);
  memcpy_bswap32(buf, hwbuf, MMCC_BUFFER_SIZE);

  return buf;
}

void *gci_mmcc_write(gci_node *node, unsigned int sector, void *buf)
{
  gci_mmcc *mmcc;
  void *hwbuf;

  mmcc = node->device_area;
  hwbuf = OFFSET(mmcc, MMCC_BUFFER_OFFSET);

  memcpy(hwbuf, buf, MMCC_BUFFER_SIZE);
  mmcc->sector_write = sector << 9;

  return buf;
}

void *gci_mmcc_write_bswap32(gci_node *node, unsigned int sector, void *buf)
{
  gci_mmcc *mmcc;
  void *hwbuf;

  mmcc = node->device_area;
  hwbuf = OFFSET(mmcc, MMCC_BUFFER_OFFSET);

  memcpy_bswap32(hwbuf, buf, MMCC_BUFFER_SIZE);
  mmcc->sector_write = sector << 9;

  return buf;
}
