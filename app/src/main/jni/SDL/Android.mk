LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE :=  SDL2
LOCAL_SRC_FILES := lib/libSDL2.so
include $(PREBUILT_SHARED_LIBRARY)