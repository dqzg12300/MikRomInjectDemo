//
// Created by ASUS on 2020/12/22.
//

#include "FileUtils.h"



using namespace std;

std::string FileUtils::getFileText(char *path,int BuffSize) {

    char buffer[BuffSize];
    memset(buffer, 0, BuffSize);
    //能用string 就用string
    std::string str;
    //int fd = open(path, O_RDONLY );
#if defined(__aarch64__)
    long fd = syscall(__NR_openat, path, O_RDONLY);
#else
    long fd = syscall(__NR_open, path, O_RDONLY);
#endif
    LOG(ERROR) << "open 内容  "<<fd;
    if(fd==-1){
        LOG(ERROR) << "读文件出错,错误原因 " << strerror(errno);
        return "";
    }
    //失败 -1；成功：>0 读出的字节数  =0文件读完了
    while (syscall(__NR_read,fd ,buffer, 1) != 0) {
        str.append(buffer);
    }
    syscall(__NR_close,fd);
    return str;
}


//string FileUtils::getRawFileText(char *path, int BuffSize) {
//    LOG(ERROR) << "执行   111";
//
//    char buffer[BuffSize];
//    memset(buffer, 0, BuffSize);
//    std::string str;
//    //int fd = open(path, O_RDONLY);
//#if defined(__aarch64__)
//    long fd = raw_syscall(__NR_openat, path, O_RDONLY);
//#else
//    long fd = raw_syscall(__NR_open, path, O_RDONLY);
//#endif
//    //失败 -1；成功：>0 读出的字节数  =0文件读完了
//    while ((read(fd, buffer, 1)) != 0) {
//
//        LOG(ERROR) << "读取文件内容  " <<buffer;
//        str.append(buffer);
//    }
//
//    syscall(__NR_close, fd);
//    return str;
//}
//


