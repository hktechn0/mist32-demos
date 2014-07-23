#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <memory.h>
#include <interrupt.h>

extern char page00[], page01[], page02[];

void __attribute__((interrupt)) int28_pagefault(void)
{
  __asm__("nop");
}

void __attribute__((interrupt)) int29_mempriv(void)
{
  __asm__("nop");
}

int main(void)
{
  idt_entry *idt;
  page_table_entry *pdt, *pt;

  unsigned int sp;
  unsigned int pagenum;

  puts("start.");

  idt = idt_setup();

  /* Page fault */
  idt_entry_setup(idt, IDT_PAGEFAULT_NUM, &int28_pagefault);
  idt_entry_enable(idt, IDT_PAGEFAULT_NUM);
  /* Invalid Privilege */
  idt_entry_setup(idt, IDT_INVALIDPRIV_NUM, &int29_mempriv);
  idt_entry_enable(idt, IDT_INVALIDPRIV_NUM);

  /* load idt */
  idt_set(idt);
  idt_load();

  printf("%s\n%s\n%s\n", page00, page01, page02);

  puts("MMU initialize start.");

  pdt = valloc(MMU_PAGE_SIZE);
  memset(pdt, 0, MMU_PAGE_SIZE);

  /* .text Page */
  printf("PDT: %p\n", pdt);
  pdt[0].flags = MMU_PTE_VALID | MMU_PTE_EX | MMU_PTE_PE;
  pdt[1].indexL1 = 1;
  pdt[1].flags = MMU_PTE_VALID | MMU_PTE_EX | MMU_PTE_PE;
  pdt[2].indexL1 = 2;
  pdt[2].flags = MMU_PTE_VALID | MMU_PTE_EX | MMU_PTE_PE;
  pdt[3].indexL1 = 3;
  pdt[3].flags = MMU_PTE_VALID | MMU_PTE_EX | MMU_PTE_PE;

  /* Stack Page */
  __asm__("srspr %0" : "=r"(sp));
  pagenum = (sp & MMU_PAGE_INDEX_L1) >> 22;
  pdt[pagenum].raw = sp & MMU_PAGE_INDEX_L1;
  pdt[pagenum].flags = MMU_PTE_VALID | MMU_PTE_PE;

  /* I/O Page */
  pdt[0x3ff].raw = 0xffffffff - MMU_PAGE_SIZE_PSE + 1;
  pdt[0x3ff].flags = MMU_PTE_VALID | MMU_PTE_PE;

  /* Test page */
  pt = valloc(MMU_PAGE_SIZE);
  memset(pt, 0, MMU_PAGE_SIZE);

  pagenum = ((unsigned int)page00 & MMU_PAGE_INDEX_L1) >> 22;
  pdt[pagenum].ptr = pt;
  pdt[pagenum].flags = MMU_PTE_VALID;

  pt[0].ptr = page02;
  pt[0].flags = MMU_PTE_VALID;
  pt[1].ptr = page01;
  pt[1].flags = MMU_PTE_VALID;
  pt[2].ptr = page00;
  pt[2].flags = MMU_PTE_VALID;

  /* start paging */
  memory_kpdt_set(pdt);
  memory_pdt_set(pdt);
  memory_paging_start();

  puts("done.");

  /* confirm */
  printf("%s\n%s\n%s\n", page00, page01, page02);

  while(1) {
    __asm__("nop");
  }
}
