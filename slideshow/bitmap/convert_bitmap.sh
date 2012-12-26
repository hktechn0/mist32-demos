#!/bin/bash

for ((N = 1; N <= 13; N += 1))
do
    NUM=`printf "%02d" ${N}`
    echo $NUM
    convert presentation/${1}${NUM}.${2} -geometry 640x480 bmp3:presentation/${NUM}_thumb.bmp
    python convert_bitmap.py presentation/${NUM}_thumb.bmp presentation/img${NUM}
    mist32-elf-objcopy -I binary -O elf32-mist32 -B mist32  presentation/img${NUM} presentation/img${NUM}.o
done
