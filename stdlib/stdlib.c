#include <stddef.h>
#include <memory.h>

void *malloc(size_t size)
{
  static volatile unsigned int memory_p = MEMORY_MAX_ADDR - STACK_SIZE;

  if(size <= 0)
    return NULL;

  memory_p = (memory_p - size) & 0xfffffffc;

  return (void *)memory_p;
}

void *memcpy(void *dest, const void *src, size_t n)
{
  unsigned int *destp;
  const unsigned int *srcp;

  unsigned int i;

  destp = dest;
  srcp = src;

  for(i = 0; i < (n >> 2); i++) {
    *destp++ = *srcp++;
  }

  return dest;
}
