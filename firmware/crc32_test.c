#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main()
{
  int fd;
  char buf[512];
  int c;
  unsigned int checksum;
  fd = open("firmware.bin", O_RDONLY);
  checksum = 0;

  while(1) {
    c = read(fd, buf, 512);
    if(c <= 0) break;

    checksum = crc32(checksum, buf, c);
    printf("%x\n", checksum);
  }

  close(fd);
}
