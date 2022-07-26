#ifndef BASE_H_
#define BASE_H_



// Context Cracking
#ifdef _WIN32
    #define OS_WINDOWS 1
#elif __APPLE__
    #define OS_MACOS 1
#elif __linux__
    #define OS_LINUX 1
#else
    #error "Failed to detect operating system!"
#endif

/* Version is 0xVVRRPPPP */
#define PREDEF_VERSION(v,r,p) (v * 1000000 + r * 10000 + p)

#ifdef _MSC_VER
    #define COMPILER_CL 1
    #if defined(_MSC_FULL_VER)
    // 193231332
    // 193  
    // 23
    // 1332
    # define COMPILER_VERSION PREDEF_VERSION(_MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, _MSC_FULL_VER % 10000)
    #else
    # if defined(_MSC_VER)
    #  define COMPILER_VERSION PREDEF_VERSION(_MSC_VER / 100, _MSC_VER % 100, 0)
    # endif
    #endif
#elif __clang__
    #define COMPILER_CLANG 1
    #define COMPILER_VERSION PREDEF_VERSION(__clang_major__,__clang_minor__,__clang_patchlevel__) 
#elif __GNUC__
    #define COMPILER_GCC 1
    # if defined(__GNUC_PATCHLEVEL__)
    #  define COMPILER_VERSION PREDEF_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
    # else
    #  define COMPILER_VERSION PREDEF_VERSION(__GNUC__, __GNUC_MINOR__, 0)
    # endif
#else
    #error "Failed to detect compiler!"
#endif


#ifdef _M_AMD64
    #define ARCH_X64 1
#elif __amd64__ || __x86_64__
    #define ARCH_X64 1
#elif __i386__
    #define ARCH_X86 1
#elif __aarch64__
    #define ARCH_ARM64 1
#elif __arm__
    #define ARCH_ARM 1
#else
    #error "Failed to detect architecture!"
#endif


#if !defined(OS_WINDOWS)
#define OS_WINDOWS 0
#endif
#if !defined(OS_MACOS)
#define OS_MACOS 0
#endif
#if !defined(OS_LINUX)
#define OS_LINUX 0
#endif
#if !defined(COMPILER_CL)
#define COMPILER_CL 0
#endif
#if !defined(COMPILER_CLANG)
#define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_GCC)
#define COMPILER_GCC 0
#endif
#if !defined(ARCH_X86)
#define ARCH_X86 0
#endif
#if !defined(ARCH_X64)
#define ARCH_X64 0
#endif
#if !defined(ARCH_ARM)
#define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
#define ARCH_ARM64 0
#endif

enum OperatingSystem
{
    kOS_Null,
    kOS_Windows,
    kOS_MacOS,
    kOS_Linux
};

enum Compiler
{
    kCompiler_Null,
    kCompiler_CL,
    kCompiler_Clang,
    kCompiler_GCC
};

enum Arch
{
    kArch_Null,
    kArch_x86,
    kArch_x64,
    kArch_ARM,
    kArch_ARM64
};

#if OS_WINDOWS
    #define OS_ENUM kOS_Windows
#elif OS_MACOS
    #define OS_ENUM kOS_MacOS
#elif OS_LINUX
    #define OS_ENUM kOS_Linux
#endif

#if COMPILER_CL
    #define COMPILER_ENUM kCompiler_CL
#elif COMPILER_CLANG
    #define COMPILER_ENUM kCompiler_Clang
#elif COMPILER_GCC
    #define COMPILER_ENUM kCompiler_GCC
#endif

#if ARCH_X86
    #define ARCH_ENUM kArch_x86
#elif ARCH_X64
    #define ARCH_ENUM kArch_x64
#elif ARCH_ARM
    #define ARCH_ENUM kArch_ARM
#elif ARCH_ARM64
    #define ARCH_ENUM kArch_ARM64
#endif

// Basic Types

#include <stdint.h>

typedef int8_t      S8;
typedef int16_t     S16;
typedef int32_t     S32;
typedef int64_t     S64;
typedef uint8_t     U8;
typedef uint16_t    U16;
typedef uint32_t    U32;
typedef uint64_t    U64;

#endif
