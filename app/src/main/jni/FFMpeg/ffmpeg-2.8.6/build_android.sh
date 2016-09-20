#!/bin/bash
export NDK=/Users/volador/Work/Library/android-ndk-r10e
export ANDROID_API_VERSION=android-21
export PLATFORM=$NDK/platforms/$ANDROID_API_VERSION/arch-arm
export PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
export PREFIX=./build
export LIB_X264_BUILD=/Users/volador/Work/ffmpeg/x264-snapshot-20160425-2245-stable/build

build_one(){
  ./configure \
--target-os=linux \
--prefix=$PREFIX \
--enable-cross-compile \
--enable-runtime-cpudetect \
--disable-asm \
--arch=arm \
--cc=$PREBUILT/darwin-x86_64/bin/arm-linux-androideabi-gcc \
--cross-prefix=$PREBUILT/darwin-x86_64/bin/arm-linux-androideabi- \
--disable-stripping \
--nm=$PREBUILT/darwin-x86_64/bin/arm-linux-androideabi-nm \
--sysroot=$PLATFORM \
--enable-gpl \
--enable-nonfree \
--enable-version3 \
--enable-shared \
--disable-static \
--enable-libx264 \
--enable-encoder=libx264 \
--disable-ffprobe \
--disable-ffplay \
--disable-ffserver \
--disable-ffmpeg \
--disable-debug \
--extra-cflags="-I$LIB_X264_BUILD/include -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -marm -march=armv7-a $CFLAGS" \
--extra-ldflags="-L$LIB_X264_BUILD/lib $LDFLAGS"
}

make clean
build_one
make -j4
make install