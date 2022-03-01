//
// Created by ASUS on 2021/3/23.
//

#ifndef INC_01_REGISTERNATIVEUTILS_H
#define INC_01_REGISTERNATIVEUTILS_H


#include <jni.h>

class RegisterNativeUtils {

public:
    static void *GetArtMethod(JNIEnv *env, jclass clazz, jmethodID methodId) {
#if __ANDROID_API__ >= __ANDROID_API_R__
        if (IsIndexId(methodId)){
            jobject method = env->ToReflectedMethod(clazz, methodId, true);
            return reinterpret_cast<void *>(env->GetLongField(method, field_art_method));
        }
#endif
        return methodId;
    }


    static int RegisterNativeAgain(JNIEnv *env, jclass clazz, void *pCallBack) ;


};


#endif //INC_01_REGISTERNATIVEUTILS_H
