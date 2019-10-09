#!/bin/sh

find src \
    \( \
           -path src/thread/generic \
        -o -path src/thread/stdcpp \
        -o -path \*/alsa \
        -o -path \*/android \
        -o -path \*/arts \
        -o -path \*/bsd \
        -o -path \*/cocoa \
        -o -path \*/coreaudio \
        -o -path \*/darwin \
        -o -path \*/direct3d11 \
        -o -path \*/directfb \
        -o -path \*/directsound \
        -o -path \*/disk \
        -o -path \*/dsp \
        -o -path \*/emscripten \
        -o -path \*/esd \
        -o -path \*/fusionsound \
        -o -path \*/haiku \
        -o -path \*/iphoneos \
        -o -path \*/jack \
        -o -path \*/kmsdrm \
        -o -path \*/mac\
        -o -path \*/macosx \
        -o -path \*/metal \
        -o -path \*/mir \
        -o -path \*/nas \
        -o -path \*/nacl \
        -o -path \*/netbsd \
        -o -path \*/pandora \
        -o -path \*/paudio \
        -o -path \*/pulseaudio \
        -o -path \*/psp \
        -o -path \*/qnx \
        -o -path \*/qsa \
        -o -path \*/raspberry \
        -o -path \*/sndio \
        -o -path \*/sun \
        -o -path \*/test \
        -o -path \*/testgui \
        -o -path \*/uikit \
        -o -path \*/vivante \
        -o -path \*/wasapi \
        -o -path \*/winmm \
        -o -path \*/winrt \
        -o -path \*/windows \
    \) \
    -prune \
    -o \
    \( -name '*.c' -o -name '*.h' \) \
    -print \
| sort
