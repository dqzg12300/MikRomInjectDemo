//
// Created by Zhenxi on 2021/5/16.
//

#include "HookUtils.h"


/**
 * Hook的整体封装,这个方法可以切换别的Hook框架
 * 先尝试用DobbyHook 如果Hook失败的话用InlineHook二次尝试
 *
 * @param dysym  被Hook函数地址
 * @param repl   替换函数
 * @param org    原始函数指针
 * @return 是否Hook成功
 */
bool HookUtils::Hooker(void *dysym, void *repl, void **org) {

    //Dobby 第三个参数需要加&
//    if (DobbyHook((void *) dysym, (void *) repl, (void **)& org) == RT_SUCCESS) {
//        return true;
//    }
//    LOG(ERROR) << "Hooker Change  InlineHook  ";
//
//    return (registerInlineHook((uint32_t) dysym, (uint32_t) repl, (uint32_t **) &org) == ELE7EN_OK);


//    LOG(ERROR) << "最终  Hooker  " ;

//    if(repl== nullptr){
//        LOG(ERROR) << "repl == null " ;
//    }
//    if(org== nullptr){
//        LOG(ERROR) << "org == null " ;
//    }
//


//    if((registerInlineHook((uint32_t) dysym, (uint32_t) repl, (uint32_t **) &org) == ELE7EN_OK)){
//        return true;
//    }

    return (DobbyHook((void *) dysym, (void *) repl, (void **)& org) == RT_SUCCESS);
}

/**
 *
 * @param dysym  被Hook函数地址
 * @param repl   替换函数
 * @param org    原始函数指针
 * @param dynSymName  函数的符号,主要为了在失败时候方便打印那个函数失败了
 * @return 是否Hook成功
 */
bool HookUtils::Hooker(void *dysym, void *repl, void **org, const char *dynSymName) {

    if (Hooker(dysym, repl, org)) {
        LOG(ERROR) << "DobbyHook  Sucess  " << dynSymName;
        return true;
    }

    LOG(ERROR) << "Hooker  Fail " << dynSymName;
    return false;
}
/**
 *
 * @param handler  SoInfo的Handler
 * @param repl   替换函数
 * @param org    原始函数指针
 * @param dynSymName  函数的符号,主要为了在失败时候方便打印那个函数失败了
 * @return 是否Hook成功
 */
bool HookUtils::Hooker(void *handler, const char *dysym, void *repl, void **org) {

    void *pSymbol = dlsym_compat(handler, dysym);

//    void *pSymbol = DobbySymbolResolver("libc.so", dysym);

//    if (ELE7EN_OK == registerInlineHook((uint32_t) 目标函数的地址,
//                                        (uint32_t) 你自己的函数的地址,
//                                        (uint32_t **) &目标函数指针,保存的函数的地址)) {
//        if (ELE7EN_OK == inlineHook((uint32_t) 目标函数的地址)) {
//            LOGE("inlineHook  success");
//        }
//    }


    if (pSymbol == nullptr) {
        LOG(ERROR) << "Hooker getDynSym get Symbol == null  error " << dysym;
        return false;
    }
    return Hooker(pSymbol, repl, org, dysym);
}
