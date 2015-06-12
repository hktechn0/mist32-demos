#include <stdio.h>
#include <stdlib.h>

#include <memory.h>
#include <io.h>
#include <debug.h>

#include "firmware.h"

void abort(void) __attribute__ ((noreturn));
int start(void) __attribute__ ((noreturn));

void abort(void)
{
  unsigned int rret;
  debug_puts("\n**** ABORT! ****\n");

  asm volatile("move %0, rret" : "=r"(rret));
  debug_puts("R31 (return pointer): ");
  debug_put_uint(rret);
  debug_putc('\n');
  debug_putc('\n');

  while(1) {
    asm volatile("nop");
  }
}

int start(void)
{
  dps_sci *sci;
  unsigned int *mmcc_buf;
  FirstSector *sector0;
  char *load_addr;
  void (*entry_addr)(void);

  unsigned int sector;
  unsigned int scancode;
  unsigned int checksum;
  unsigned int max_sector, size_remaining, size_copy;

  gci_setup();

  /* SCI Setup */
  sci = dps_sci_init();
  /* clear buffer */
  dps_sci_clear(sci, true, true);
  /* 9600bps, no interrupt */
  dps_sci_setup(sci, true, true, 0, 0, 0);

  debug_init(sci);
  debug_puts("****************************************");
  debug_puts("****************************************\n");
  debug_puts("*************** ");
  debug_puts("Open Design Computer Project - MIST32 Processor");
  debug_puts(" ****************\n");
  debug_puts("****************************************");
  debug_puts("****************************************\n");

  debug_puts("RAM Total: ");
  debug_put_uint(dps_mi_memsize());
  debug_putc('\n');

  debug_puts("Booting...\n");
  
  /* MMCC Setup */
  debug_puts("* Check MMCC: ");
  gci_mmcc_init(&gci_nodes[GCI_MMCC_NUM]);
  mmcc_buf = malloc(MMCC_BUFFER_SIZE);
  /* test read */
  sector0 = malloc(sizeof(FirstSector));
  gci_mmcc_read_bswap32(&gci_nodes[GCI_MMCC_NUM], 0, sector0->data);
  debug_puts("OK.\n");

  debug_puts("* Boot Sector Checksum: ");
  debug_put_uint(crc32(0, sector0->data, 512));
  debug_putc('\n');
  debug_puts("Loading binary...\n");

  /* Signature check */
  if(sector0->header.signature != BOOT_SIGNATURE) {
    debug_puts("ERROR: INVALID BOOT SIGNATURE. ");
    debug_put_uint(sector0->header.signature);
    debug_putc('\n');
    abort();
  }
  debug_puts("* Boot signature validation: OK.\n");

  /* Dump boot sector */
  debug_mem(sector0->data, 512);

  debug_puts("* Version: ");
  debug_put_uint(sector0->header.version);
  debug_puts("Start sector: ");
  debug_put_uint(sector0->header.bin_start_sector);
  debug_putc('\n');

  debug_puts("* Size: ");
  debug_put_uint(sector0->header.bin_size);
  debug_puts("Checksum: ");
  debug_put_uint(sector0->header.bin_checksum);
  debug_putc('\n');

  /* CRC check */
  checksum = 0;
  max_sector = ((sector0->header.bin_size + 511) / 512);
  size_remaining = sector0->header.bin_size;
  for(sector = sector0->header.bin_start_sector; sector <= max_sector; sector++) {
    gci_mmcc_read_bswap32(&gci_nodes[GCI_MMCC_NUM], sector, mmcc_buf);
    size_copy = size_remaining >= 512 ? 512 : size_remaining;
    checksum = crc32(checksum, mmcc_buf, size_copy);
    size_remaining -= size_copy;
  }

  if(sector0->header.bin_checksum != checksum) {
    debug_puts("ERROR: INVALID CHECKSUM. ");
    debug_put_uint(checksum);
    debug_put_uint(sector0->header.bin_checksum);
    debug_putc('\n');
    abort();
  }
  debug_puts("* Checksum validation: OK.\n");

  debug_puts("* Load: ");
  debug_put_uint(sector0->header.bin_load_addr);
  debug_puts("Entry: ");
  debug_put_uint(sector0->header.bin_entry_addr);
  debug_putc('\n');
  load_addr = (void *)(sector0->header.bin_load_addr);
  entry_addr = (void *)(sector0->header.bin_entry_addr);

  /* Load binary */
  debug_puts("* Loading");
  for(sector = sector0->header.bin_start_sector; sector <= max_sector; sector++) {
    gci_mmcc_read_bswap32(&gci_nodes[GCI_MMCC_NUM], sector, load_addr);
    load_addr += 512;
    debug_putc('.');
  }
  debug_puts(" OK.\n");

  /* jump */
  debug_puts("Starting loaded program...\n");
  //debug_puts("* Press any key to boot.");
  entry_addr();

  while(1) {
    scancode = gci_kmc_scancode(&gci_nodes[GCI_KMC_NUM]);
    if(scancode & KMC_SCANCODE_VALID) {
      debug_put_uint(scancode);
    }
  }
}
