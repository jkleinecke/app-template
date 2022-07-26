#ifndef BASE_H_
#define BASE_H_

#if !defined(NDEBUG)
#define INTERNAL_BUILD 1
#define ENABLE_ASSERT 1
#endif

#if !defined(INTERNAL_BUILD)
#define INTERNAL_BUILD 0
#endif

#if !defined(ENABLE_ASSERT)
#define ENABLE_ASSERT 0
#endif

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

//=====================
// Utility Macros
//=====================

#define Stmnt(S) do{ S }while(0)

#if ENABLE_ASSERT
    #if !defined(AssertBreak)
        #define AssertBreak() (*(volatile int*)0 = 0)
    #endif
    #define Assert(c) Stmnt( if(!c){ AssertBreak(); })
#else
    #define AssertBreak()
    #define Assert(c)
#endif

#define StaticAssert(c,l) typedef U8 Glue(l,__LINE__) [(c)?1:-1]

#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)
#define Glue_(A,B) A##B
#define Glue(A,B) Glue_(A,B)

#define ArrayCount(a) (sizeof(a)/sizeof(*(a)))

#define IntFromPtr(p) (U64)((U8*)p - (U8*)0)
#define PtrFromInt(n) (void*)((U8*)0 + (n))

#define Member(T,m) (((T*)0)->m)
#define OffsetOfMember(T,m) IntFromPtr(&Member(T,m))


#define Min(a,b) (((a)<(b))?(a):(b))
#define Max(a,b) (((a)>(b))?(a):(b))
#define Clamp(a,x,b) (((x)<(a))?(a):\
((b)<(x))?(b):(x))
#define ClampTop(a,b) Min(a,b)
#define ClampBot(a,b) Max(a,b)

#define AlignUpPow2(x,p) (((x) + (p) - 1)&~((p) - 1))
#define AlignDownPow2(x,p) ((x)&~((p) - 1))

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) ((x) << 40)

#define Thousand(x) ((x)*1000)
#define Million(x)  ((x)*1000000llu)
#define Billion(x)  ((x)*1000000000llu)
#define Trillion(x) ((x)*1000000000000llu)

#define global static
#define local  static
#define function static

#if COMPILER_CL
# define threadvar __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
# define threadvar __thread
#else
# error threadvar defined for this compiler
#endif

#define c_linkage_begin extern "C"{
#define c_linkage_end   }
#define c_linkage extern "C"

#if COMPILER_CL
# define shared_export __declspec(dllexport)
#elif COMPILER_CLANG || COMPILER_GCC
# define shared_export __attribute__((dllexport))
#else
# error shared_export not defined for this compiler
#endif

#define shared_function c_linkage shared_export

#include <string.h>
#define MemoryZero(p,z) memset((p), 0, (z))
#define MemoryZeroStruct(p) MemoryZero((p), sizeof(*(p)))
#define MemoryZeroArray(p)  MemoryZero((p), sizeof(p))
#define MemoryZeroTyped(p,c) MemoryZero((p), sizeof(*(p))*(c))

#define MemoryMatch(a,b,z) (memcmp((a),(b),(z)) == 0)

#define MemoryCopy(d,s,z) memmove((d), (s), (z))
#define MemoryCopyStruct(d,s) MemoryCopy((d),(s),\
Min(sizeof(*(d)),sizeof(*(s))))
#define MemoryCopyArray(d,s)  MemoryCopy((d),(s),Min(sizeof(s),sizeof(d)))
#define MemoryCopyTyped(d,s,c) MemoryCopy((d),(s),\
Min(sizeof(*(d)),sizeof(*(s)))*(c))

//=====================
// Basic Types
//=====================

#include <stdint.h>

typedef int8_t      S8;
typedef int16_t     S16;
typedef int32_t     S32;
typedef int64_t     S64;
typedef uint8_t     U8;
typedef uint16_t    U16;
typedef uint32_t    U32;
typedef uint64_t    U64;
typedef S8          B8;
typedef S16         B16;
typedef S32         B32;
typedef S64         B64;
typedef float       F32;
typedef double      F64;

#endif
