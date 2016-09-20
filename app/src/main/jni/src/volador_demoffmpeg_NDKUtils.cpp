//
// Created by Volador on 16/2/22.
//

#include "volador_demoffmpeg_NDKUtils.h"

#include "my_log.h"

extern "C" {
    #include "libavcodec/avcodec.h"
}


JNIEXPORT jstring JNICALL Java_volador_demoffmpeg_NDKUtils_sayHello
        (JNIEnv *env, jobject obj) {

    LOGD("JNI TEST UPLOAD");

    return (*env).NewStringUTF(avcodec_license());

}




