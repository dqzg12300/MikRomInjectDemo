#include <cinttypes>
#include <sys/mman.h>
#include "pmparser.h"
#include "logging.h"
#include "wrap.h"

#include "android/log.h"
#include "hide.h"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "hide", __VA_ARGS__);

#define LOGE(...) __android_log_print(ANDROID_LOG_DEBUG, "hide", __VA_ARGS__);
/**
 * Magic to hide from /proc/###/maps, the idea is from Haruue Icymoon (https://github.com/haruue)
 */
//#ifdef __LP64__
//#define LIB_PATH "/system/lib64/"
//#else
//#define LIB_PATH "/system/lib/"
//#endif
#define LIB_PATH ""
struct hide_struct {
    procmaps_struct *original;
    uintptr_t backup_address;
};

static int get_prot(const procmaps_struct *procstruct) {
    int prot = 0;
    if (procstruct->is_r) {
        prot |= PROT_READ;
    }
    if (procstruct->is_w) {
        prot |= PROT_WRITE;
    }
    if (procstruct->is_x) {
        prot |= PROT_EXEC;
    }
    return prot;
}

static int do_hide(hide_struct *data) {
    auto procstruct = data->original;
    auto start = (uintptr_t) procstruct->addr_start;
    auto end = (uintptr_t) procstruct->addr_end;
    auto length = end - start;
    int prot = get_prot(procstruct);

    // backup
    data->backup_address = (uintptr_t) _mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (data->backup_address == (uintptr_t) MAP_FAILED) {
        return 1;
    }
    LOGD("%" PRIxPTR"-%" PRIxPTR" %s %ld %s is backup to %" PRIxPTR, start, end, procstruct->perm, procstruct->offset, procstruct->pathname,
         data->backup_address);

    if (!procstruct->is_r) {
        LOGD("mprotect +r");
        _mprotect((void *) start, length, prot | PROT_READ);
    }
    LOGD("memcpy -> backup");
    memcpy((void *) data->backup_address, (void *) start, length);

    // munmap original
    LOGD("munmap original");
    munmap((void *) start, length);

    // restore
    LOGD("mmap original");
    _mmap((void *) start, length, prot, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    LOGD("mprotect +w");
    _mprotect((void *) start, length, prot | PROT_WRITE);
    LOGD("memcpy -> original");
    memcpy((void *) start, (void *) data->backup_address, length);
    if (!procstruct->is_w) {
        LOGD("mprotect -w");
        _mprotect((void *) start, length, prot);
    }
    return 0;
}

int riru_hide(const char **names, int names_count) {
    int pid=getpid();
    procmaps_iterator *maps = pmparser_parse(pid);
    if (maps == nullptr) {
        LOGE("cannot parse the memory map %d",pid);
        return false;
    }

//    char buf[PATH_MAX];
    hide_struct *data = nullptr;
    size_t data_count = 0;
    procmaps_struct *maps_tmp;
    while ((maps_tmp = pmparser_next(maps)) != nullptr) {
        bool matched = false;
        LOGD("fuck %s",maps_tmp->pathname);
        for (int i = 0; i < names_count; ++i) {
//            snprintf(buf, PATH_MAX, LIB_PATH "%s", names[i]);
            if (strcmp(maps_tmp->pathname, names[i]) == 0) {
                matched = true;
                break;
            }
        }
        if (!matched) continue;

        auto start = (uintptr_t) maps_tmp->addr_start;
        auto end = (uintptr_t) maps_tmp->addr_end;
        if (maps_tmp->is_r) {
            if (data) {
                data = (hide_struct *) realloc(data, sizeof(hide_struct) * (data_count + 1));
            } else {
                data = (hide_struct *) malloc(sizeof(hide_struct));
            }
            data[data_count].original = maps_tmp;
            data_count += 1;
        }
        LOGD("%" PRIxPTR"-%" PRIxPTR" %s %ld %s", start, end, maps_tmp->perm, maps_tmp->offset, maps_tmp->pathname);
    }

    for (int i = 0; i < data_count; ++i) {
        do_hide(&data[i]);
    }

    if (data) free(data);
    pmparser_free(maps);
    return 0;
}