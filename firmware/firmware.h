#include <stddef.h>
#include <stdint.h>

/* signature: MIST (big endian) */
#define BOOT_SIGNATURE 0x6d697374

struct boot_header {
  unsigned int signature;
  unsigned int version;
  //unsigned int __dummy;
  unsigned int entry_n;

  unsigned int bin_start_sector;
  unsigned int bin_size;
  unsigned int bin_load_addr;
  unsigned int bin_entry_addr;
  unsigned int bin_checksum;
};

typedef union first_sector {
  struct boot_header header;
  unsigned char data[MMCC_SECTOR_SIZE];
} FirstSector;

uint32_t crc32(uint32_t crc, const void *buf, size_t size);
