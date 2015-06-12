#!/usr/bin/env python

import sys
import os
from subprocess import Popen, PIPE
import struct

BOOT_SIGNATURE = 0x6d697374

if __name__ == "__main__":
    infile = sys.argv[1]
    outfile = sys.argv[2]

    # crc32 from libarchive-zip-perl
    cksum = Popen(["crc32", infile], stdout=PIPE).communicate()[0]
    cksum = int(cksum, 16)
    size = os.path.getsize(infile)

    print("Size: %x" % size)
    print("Checksum: %x" % cksum)

    start_addr = 0x10000

    with open(outfile, "wb") as f:
        header = struct.pack(">IIIIIIII",
                             BOOT_SIGNATURE, # signature
                             0,              # version
                             1,              # entry_n
                             1,              # bin_start_sector
                             size,           # bin_size
                             start_addr,     # bin_load_addr
                             start_addr,     # bin_entry_addr
                             cksum)          # bin_checksum
        f.write(header)

    ddout = Popen(["dd", "if=%s" % infile, "of=%s" % outfile,
                   "bs=512", "seek=1", "conv=notrunc"], stdout=PIPE).communicate()[0]
    print(ddout)
