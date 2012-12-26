import Image
import sys

if __name__ == "__main__":
    im = Image.open(sys.argv[1])
    out = open(sys.argv[2], "wb")

    if len(im.getdata()) != 640 * 480:
        exit()

    for p in im.getdata():
        # print p
        pixel = (p[2] >> 3) & 0x1f
        pixel |= ((p[1] >> 2) & 0x3f) << 5
        pixel |= ((p[0] >> 3) & 0x1f) << 11
        pixel &= 0xffff
        
        out.write(chr(pixel >> 8))
        out.write(chr(pixel & 0xff))

    out.close()
