//
// Created by Volador on 16/3/9.
//

#ifndef DEMOFFMPEG_MY_LOG_H_H
#define DEMOFFMPEG_MY_LOG_H_H

#include <android/log.h>

#define TAG "JNI-FFMpeg" // 这个是自定义的LOG的标识
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,TAG,__VA_ARGS__) // 定义LOGD类型
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


#define LOGV1(...) __android_log_print(ANDROID_LOG_VERBOSE,__VA_ARGS__,__VA_ARGS__)

#endif //DEMOFFMPEG_MY_LOG_H_H
