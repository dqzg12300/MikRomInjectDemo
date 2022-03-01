
# Sets the minimum version of CMake required to build the native library.
#指定 需要CMAKE的 最小版本要求
cmake_minimum_required(VERSION 3.4.1)


set(DobbyHome /Users/king/git_src/Dobby)

#添加内联支持
enable_language(C ASM)

include_directories(
        src/main/jni/dlfc
        src/main/jni/utils
)

#添加一个 远程链接库
add_library( # Sets the name of the library. (我们用C/C++ 文件生成的 动态库 so名字 )
        mikDobby

        # Sets the library as a shared library.(表明用于共享的 动态库 so结尾)
        SHARED
        src/main/jni/main.cpp
        #解析的工具类
        src/main/jni/utils/parse.cpp

        )

find_library( # Sets the name of the path variable.（调用系统的日志库）
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


# 在A启动的时候进行B的关联
target_link_libraries( # Specifies the target library. 链接 我们自己生成的库 名字要和add_library 一样
        mikDobby
        dobby

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        )


#设置路径,引入Dobby
include_directories(
        ${DobbyHome}/include
        ${DobbyHome}/source
        ${DobbyHome}/builtin-plugin
        ${DobbyHome}/builtin-plugin/AndroidRestriction
        ${DobbyHome}/builtin-plugin/SymbolResolver
        ${DobbyHome}/external/logging
)


macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

SET_OPTION(DOBBY_DEBUG ON)
SET_OPTION(DOBBY_GENERATE_SHARED ON)
SET_OPTION(Plugin.LinkerLoadCallback OFF)

add_subdirectory(/Users/king/git_src/Dobby dobby.build)

if(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
    add_definitions(-DCORE_SO_NAME="${LIBRARY_NAME}")
elseif(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "armeabi-v7a")
    add_definitions(-DCORE_SO_NAME="${LIBRARY_NAME}")
endif()




