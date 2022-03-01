

#include "main.h"

int (*source_openat)(int fd, const char *path, int oflag, int mode) = nullptr;

int MyOpenAt(int fd, const char *pathname, int flags, int mode) {
    LOG(ERROR) << "mik MyOpenAt  pathname   "<<pathname;
    if (strcmp(pathname, "/sbin/su") == 0 || strcmp(pathname, "/system/bin/su") == 0) {
        pathname = "/system/xbin/Mysu";
    }
    return source_openat(fd, pathname, flags, mode);
}


int (*source_execve)(const char *filename, char *const argv[], char *const envp[]) = nullptr;

int MyExecve(const char *filename, char *const argv[], char *const envp[]) {
    LOG(ERROR) << "mik MyExecve filename "<<filename;
    //filename = "MySu";

    if (strcmp(filename, "su") == 0) {
        filename = "Mysu";
    }

    return source_execve(filename, argv, envp);
}

void HookExecve() {

    void *execve = DobbySymbolResolver("/system/lib/libc.so", "execve");
    if (execve == nullptr) {
        LOG(ERROR) << "execve null ";
        return;
    }

    LOG(ERROR) << "拿到 execve 地址 ";

    //dobby
    if (DobbyHook((void *) execve,
                  (void *) MyExecve,
                  (void **) &source_execve) == RT_SUCCESS) {
        LOG(ERROR) << "DobbyHook execve sucess";
    }
}

void HookOpenAt() {
    void *__openat =
            DobbySymbolResolver("libc.so", "__openat");

    if (__openat == nullptr) {
        LOG(ERROR) << "__openat null ";
        return;
    }

    LOG(ERROR) << "拿到 __openat 地址 ";

    //dobby
    if (DobbyHook((void *) __openat,
                  (void *) MyOpenAt,
                  (void **) &source_openat) == RT_SUCCESS) {
        LOG(ERROR) << "DobbyHook __openat sucess";
    }
}


jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    LOG(ERROR) << "Test JNI_OnLoad 开始加载";
    //在 onload 改变 指定函数 函数地址 替换成自己的
    JNIEnv *env = nullptr;

    HookExecve();
    HookOpenAt();
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_OK) {
        return JNI_VERSION_1_6;
    }
    return 0;

}

