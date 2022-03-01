//
// Created by ASUS on 2020/12/22.
//

#ifndef INC_01_FILEUTILS_H
#define INC_01_FILEUTILS_H

#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex>
#include <bits/getopt.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <asm/fcntl.h>
#include <dobby.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/stat.h>

#include "Log.h"
#include "logging.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//关联
extern "C" long raw_syscall(long __number, ...);

class FileUtils {
    public:
        static std::string getFileText(char * path,int BuffSize);
        static std::string getRawFileText(char * path,int BuffSize);

};


#endif //INC_01_FILEUTILS_H
