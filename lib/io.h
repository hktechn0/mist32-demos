static inline __attribute__((always_inline)) void *iosr_read(void)
{
  void *io;

  __asm__("sriosr %0" : "=r"(io));

  return io;
}

static inline __attribute__((always_inline)) void frcr_read(unsigned int *hi, unsigned int *lo)
{
  __asm__("srfrcr     \n\t"
	  "srfrchr %0 \n\t"
	  "srfrclr %1"
	  : "=r"(*hi), "=r"(*lo));
}

#include "dps.h"
#include "gci.h"
