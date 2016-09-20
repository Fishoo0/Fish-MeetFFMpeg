#!/bin/bash
export NDK=/Users/volador/Work/Library/android-ndk-r10e
export PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
export PLATFORM=$NDK/platforms/android-21/arch-arm
export PREFIX=../simplefflib
build_one(){
  ./configure --target-os=linux --prefix=$PREFIX \
--enable-cross-compile \
--enable-runtime-cpudetect \
--disable-asm \
--arch=arm \
--cc=$PREBUILT/darwin-x86_64/bin/arm-linux-androideabi-gcc \
--cross-prefix=$PREBUILT/darwin-x86_64/bin/arm-linux-androideabi- \
--disable-stripping \
--nm=$PREBUILT/darwin-x86_64/bin/arm-linux-androideabi-nm \
--sysroot=$PLATFORM \
--enable-gpl --enable-shared --disable-static --enable-small \
--disable-ffprobe --disable-ffplay --disable-ffmpeg --disable-ffserver --disable-debug \
--extra-cflags="-fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -marm -march=armv7-a" 
}

build_one

make
make install
