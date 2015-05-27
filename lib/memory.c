#include <stdio.h>
#include "memory.h"

void *memcpy_bswap32(void *dest, const void *src, size_t n)
{
  unsigned int *destp;
  const unsigned int *srcp;

  unsigned int i;

  destp = dest;
  srcp = src;

  for(i = 0; i < (n >> 2); i++) {
    *destp = __builtin_bswap32(*srcp);
    destp++;
    srcp++;
  }

  return dest;
}
