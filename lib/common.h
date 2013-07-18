#include <stddef.h>
#include <stdbool.h>

#define MEMORY_MAX_ADDR 0x04000000
#define STACK_SIZE 0x100000 /* 1MB */

#define IMAGE_MAX 13
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_DEPTH 16
#define IMAGE_SIZE ((IMAGE_DEPTH / 8) * IMAGE_WIDTH * IMAGE_HEIGHT)

#define EOF -1

#define OFFSET(ptr, offset) ((void *)((char *)ptr + offset))

typedef unsigned int size_t;

#define bswap32 __builtin_bswap32

void *malloc(size_t size);
void memcpy_bswap32(void *dest, const void *src, size_t n);

/*
static inline uint32_t bswap32(uint32_t int32)
{
  __asm__("rev8 %0, %1" : "=r"(int32) : "r"(int32));
  return int32;
}

static inline uint32_t bswap32(uint32_t int32)
{
  int32 = (int32 >> 24) | (int32 << 24) | ((int32 >> 8) & 0xff00) | ((int32 << 8) & 0xff0000);
  return int32;
}
*/
