//
// Created by Zhenxi on 2021/5/16.
//
#include "logging.h"
#include <dobby.h>
//#include "inlineHook.h"
#include <dlfcn_compat.h>


#ifndef VMP_HOOKUTILS_H
#define VMP_HOOKUTILS_H


class HookUtils {
public:
    static bool Hooker(void * dysym,void * repl,void ** org);
    static bool Hooker(void* lib, const char *dysym, void * repl, void ** org);
    static bool Hooker(void *dysym, void *repl, void **org,const char *dynSymName);
};


#endif //VMP_HOOKUTILS_H
