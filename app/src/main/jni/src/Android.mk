LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE := demoffmpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../SDL/include \
                        $(LOCAL_PATH)/../FFMpeg/include

LOCAL_SRC_FILES := \
                        ../SDL/src/main/android/SDL_android_main.c\
                        android_player.c \
                        lesson1.c


LOCAL_LDLIBS := -llog -ljnigraphics -lz -landroid -lm -pthread -lGLESv1_CM -lGLESv2

LOCAL_SHARED_LIBRARIES := avcodec-56 avcodec avfilter-5 avfilter avformat-56 avformat avutil-54 avutil avswresample-1 avswresample swscale-3 swscale avdevice libavdevice-56 libavdevice libpostproc-53 libpostproc SDL2
 
include $(BUILD_SHARED_LIBRARY)












