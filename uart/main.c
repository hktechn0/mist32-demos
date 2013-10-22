#include <stdio.h>
#include <stdlib.h>

#include <memory.h>
#include <io.h>
#include <interrupt.h>
#include <debug.h>

dps_sci *sci;
dps_utim64 *timer;

volatile unsigned int tick;
volatile char buf[SCI_RX_BUFFER_SIZE];
volatile unsigned int buf_start, buf_end;

void __attribute__((interrupt)) int05_keyboard(void)
{
  unsigned int scancode;

  gci_interrupt_ack(&gci_nodes[GCI_KMC_NUM]);
  scancode = gci_kmc_scancode(&gci_nodes[GCI_KMC_NUM]);
  debug_put_uint(scancode);
}

void __attribute__((interrupt)) int24_utim64(void)
{
  unsigned int flags;

  /* read which is ticked and ack interrupt */
  flags = dps_utim64_flags();

  if(flags & UTIM64FLAGS_A0) {
    tick++;
  }
}

void __attribute__((interrupt)) int25_dpsls(void)
{
  unsigned int flags;
  int c;

  flags = dps_lsflags();

  if(flags & DPS_LSFLAGS_SCIRIE) {
    while((c = sci_getc(sci)) != EOF) {
      buf[buf_end++] = c;

      if(buf_end >= SCI_RX_BUFFER_SIZE) {
	buf_end = 0;
      }
    }
  }
}

int start(void)
{
  idt_entry *idt;

  tick = 0;
  buf_start = 0;
  buf_end = 0;

  malloc(STACK_SIZE);

  idt = idt_setup();

  /* keybord */
  idt_entry_setup(idt, IDT_GCI_KMC_NUM, &int05_keyboard);
  idt_entry_enable(idt, IDT_GCI_KMC_NUM);
  /* DPS Timer */
  idt_entry_setup(idt, IDT_DPS_UTIM64_NUM, &int24_utim64);
  idt_entry_enable(idt, IDT_DPS_UTIM64_NUM);
  /* DPS lowspeed device for UART */
  idt_entry_setup(idt, IDT_DPS_LS_NUM, &int25_dpsls);
  idt_entry_enable(idt, IDT_DPS_LS_NUM);

  /* load idt */
  idt_set(idt);
  idt_load();

  /* gci setup */
  gci_setup();
  interrupt_enable();

  /* setup serial */
  sci = dps_sci_init();
  dps_sci_clear(sci, true, true);
  dps_sci_setup(sci, true, true, 0, 0, 1); /* 9600bps */
  debug_init(sci);

  debug_puts("OK.\n");
  debug_puts("MEM: ");
  debug_put_uint(dps_mi_memsize());
  debug_putc('\n');

  sci_putc(sci, 'O');
  sci_putc(sci, 'K');
  sci_putc(sci, '.');
  sci_putc(sci, '\r');
  sci_putc(sci, '\n');

  /* setup timer */
  timer = dps_utim64_init(0); /* timer A */
  dps_utim64_disable(timer);
  timer->mc[0] = 0;
  timer->mc[1] = 0;
  /* 49.152mhz / 1024 = 48khz / 48 / 1000 = 1hz */
  timer->cc[0][0] = 0;
  timer->cc[0][1] = 1024 * 48 * 1000;
  /* Enable, Interrupt Enable, Periodic mode */
  timer->cccfg[0] = UTIM64CFG_ENA | UTIM64CFG_IE | UTIM64CFG_MODE;
  dps_utim64_enable(timer);

  while(1) {
    if(tick > 0) {
      sci_putc(sci, '_');
      tick--;
    }

    while(buf_start != buf_end) {
      sci_putc(sci, buf[buf_start++]);

      if(buf_start >= SCI_RX_BUFFER_SIZE) {
	buf_start = 0;
      }
    }
  }
}
