#include <stddef.h>
#include <stdbool.h>

#define EOF -1

#define MEMORY_MAX_ADDR 0x04000000
#define STACK_SIZE 0x4000

#define IMAGE_MAX 13

typedef unsigned int size_t;

void *malloc(size_t size);
