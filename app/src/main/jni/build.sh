#!/usr/bin/env bash
#This is shell for ndk-building
#garanteed ndk version android-ndk-r10e

ndk-build NDK_LOG=1 \
        NDK_PROJECT_PATH=null \
        APP_BUILD_SCRIPT=./Android.mk \
        NDK_APPLICATION_MK=./Application.mk \
        NDK_OUT=../../../build/intermediates/ndk/debug/obj \
        NDK_LIBS_OUT=../../../build/intermediates/ndk/debug/lib \
        APP_PLATFORM=android-23
