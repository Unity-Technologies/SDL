#!/bin/sh

FILES=$(find src -name '*.c' -o -name '*.h' | sort)

# Blacklist of files to exclude
EXCLUDE="/thread/generic /thread/stdcpp /disk /iphoneos /bsd /windows /psp /android /haiku /nacl /winrt /emscripten /cocoa /test /nas /qsa /esd /netbsd /sndio /pulseaudio /winmm /paudio /alsa /coreaudio /xaudio2 /jack /dsp /wasapi /arts /fusionsound /directsound /sun /macosx /windows /uikit /pandora /raspberry /qnx /vivante /kmsdrm /mir /directfb /direct3d11 /direct3d /darwin"
for filter in $EXCLUDE; do
    FILES=$(echo "$FILES" | grep -v $filter)
done

# Output
for file in $FILES; do
    echo $file
done


