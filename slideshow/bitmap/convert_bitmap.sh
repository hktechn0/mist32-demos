#!/bin/bash

# USAGE
# ./convert_bitmap.sh <image_file_prefix> <extention> <start> <end>

# SAMPLE
# image1.png, image2.png...
# ./convert_bitmap.sh image png

DIR="presentation"
truncate -s 0 ${DIR}/mmc.img

i=1

for ((N = $3; N <= $4; N += 1))
do
    # input number format
    NUMI=`printf "%d" ${N}`

    # output number format
    NUM=`printf "%02d" ${i}`
    i=$[$i + 1]

    echo $NUM

    # convert to bitmap
    convert -verbose ${DIR}/${1}${NUMI}.${2} -geometry 640x480 bmp3:${DIR}/${NUM}_thumb.bmp

    # convert to raw bitmap
    python convert_bitmap.py ${DIR}/${NUM}_thumb.bmp ${DIR}/img${NUM}

    # output to MMC image
    cat ${DIR}/img${NUM} >> ${DIR}/mmc.img
    sleep 1

    # output to ELF format
    mist32-elf-objcopy -I binary -O elf32-mist32 -B mist32  ${DIR}/img${NUM} ${DIR}/img${NUM}.o
done
