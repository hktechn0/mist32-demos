#include <stdlib.h>
#include "interrupt.h"

idt_entry *idt_setup(void)
{
  idt_entry *idt;
  int i;

  idt = malloc(sizeof(idt_entry) * IDT_ENTRY_MAX);

  for(i = 0; i < IDT_ENTRY_MAX; i++) {
    idt[i].flags = 0;
    idt[i].handler = 0;
  }

  return idt;
}

void idt_entry_setup(idt_entry *idt, unsigned int num, void (*handler)(void))
{
  idt[num].flags = IDT_FLAGS_VALID;
  idt[num].handler = handler;
}

void idt_entry_enable(idt_entry *idt, unsigned int num)
{
  idt[num].flags |= IDT_FLAGS_ENABLE;
}
