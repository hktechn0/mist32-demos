#include "common.h"

void *malloc(size_t size)
{
  static unsigned int memory_p = MEMORY_MAX_ADDR;
  
  memory_p = (memory_p - size) & ~(0x3);

  return (void *)memory_p;
}
