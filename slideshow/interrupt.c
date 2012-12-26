#include "common.h"

inline void interrupt_enable(void)
{
  __asm__("srieiw 1");
}

inline void interrupt_disable(void)
{
  __asm__("srieiw 0");
}

inline void idt_set(idt_entry *idtr)
{
  __asm__("sridtw %0"
	:
	:"r"(idtr)
	);
}

inline void idt_load(void)
{
  __asm__("idts");
}


idt_entry *idt_setup(void)
{
  volatile idt_entry *idt;
  int i;

  idt = malloc(sizeof(idt_entry) * IDT_ENTRY_MAX);

  for(i = 0; i < IDT_ENTRY_MAX; i++) {
    idt[i].flags = 0;
    idt[i].handler = 0;
  }

  return idt;
}

void idt_entry_setup(volatile idt_entry *idt, unsigned int num, void (*handler)(void))
{
  idt[num].flags = IDT_FLAGS_VALID;
  idt[num].handler = handler;
}

void idt_entry_enable(volatile idt_entry *idt, unsigned int num)
{
  idt[num].flags |= IDT_FLAGS_ENABLE;
}
