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
// #define PREDEF_VERSION(v,r,p) (v * 1000000 + r * 10000 + p)
#define PREDEF_VERSION(v,r,p) Stringify(v.r.p)

#ifdef _MSC_VER
    #define COMPILER_CL 1
    #if defined(_MSC_FULL_VER)
    // 193231332
    // 193  
    // 23
    // 1332
    // # define COMPILER_VERSION PREDEF_VERSION(_MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, _MSC_FULL_VER % 10000)
    # define COMPILER_VERSION Stringify(_MSC_FULL_VER)
    #else
    # if defined(_MSC_VER)
    #  define COMPILER_VERSION Stringify(_MSC_VER)
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

////////////////////////////////
// Linked List Macros

#define DLLPushBack_NP(f,l,n,next,prev) ((f)==0?\
((f)=(l)=(n),(n)->next=(n)->prev=0):\
((n)->prev=(l),(l)->next=(n),(l)=(n),(n)->next=0))
#define DLLPushBack(f,l,n) DLLPushBack_NP(f,l,n,next,prev)

#define DLLPushFront(f,l,n) DLLPushBack_NP(l,f,n,prev,next)

#define DLLRemove_NP(f,l,n,next,prev) ((f)==(n)?\
((f)==(l)?\
((f)=(l)=(0)):\
((f)=(f)->next,(f)->prev=0)):\
(l)==(n)?\
((l)=(l)->prev,(l)->next=0):\
((n)->next->prev=(n)->prev,\
(n)->prev->next=(n)->next))
#define DLLRemove(f,l,n) DLLRemove_NP(f,l,n,next,prev)

#define SLLQueuePush_N(f,l,n,next) ((f)==0?\
(f)=(l)=(n):\
((l)->next=(n),(l)=(n)),\
(n)->next=0)
#define SLLQueuePush(f,l,n) SLLQueuePush_N(f,l,n,next)

#define SLLQueuePushFront_N(f,l,n,next) ((f)==0?\
((f)=(l)=(n),(n)->next=0):\
((n)->next=(f),(f)=(n)))
#define SLLQueuePushFront(f,l,n) SLLQueuePushFront_N(f,l,n,next)

#define SLLQueuePop_N(f,l,next) ((f)==(l)?\
(f)=(l)=0:\
(f)=(f)->next)
#define SLLQueuePop(f,l) SLLQueuePop_N(f,l,next)

#define SLLStackPush_N(f,n,next) ((n)->next=(f),(f)=(n))
#define SLLStackPush(f,n) SLLStackPush_N(f,n,next)

#define SLLStackPop_N(f,next) ((f)==0?0:\
(f)=(f)->next)
#define SLLStackPop(f) SLLStackPop_N(f,next)

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

//=====================
// Variadic Functions
//=====================

#include <stdarg.h>

//=====================
// Basic Constants
//=====================

global S8  min_S8  = (S8) 0x80;
global S16 min_S16 = (S16)0x8000;
global S32 min_S32 = (S32)0x80000000;
global S64 min_S64 = (S64)0x8000000000000000llu;

global S8  max_S8  = (S8) 0x7f;
global S16 max_S16 = (S16)0x7fff;
global S32 max_S32 = (S32)0x7fffffff;
global S64 max_S64 = (S64)0x7fffffffffffffffllu;

global U8  max_U8  = 0xff;
global U16 max_U16 = 0xffff;
global U32 max_U32 = 0xffffffff;
global U64 max_U64 = 0xffffffffffffffffllu;

global F32 machine_epsilon_F32 = 1.1920929e-7f;
global F32 pi_F32  = 3.14159265359f;
global F32 tau_F32 = 6.28318530718f;
global F32 e_F32 = 2.71828182846f;
global F32 gold_big_F32 = 1.61803398875f;
global F32 gold_small_F32 = 0.61803398875f;

global F64 machine_epsilon_F64 = 2.220446e-16;
global F64 pi_F64  = 3.14159265359;
global F64 tau_F64 = 6.28318530718;
global F64 e_F64 = 2.71828182846;
global F64 gold_big_F64 = 1.61803398875;
global F64 gold_small_F64 = 0.61803398875;

//=====================
// Symbolic Constants
//=====================

enum Axis
{
    kAxis_X,
    kAxis_Y,
    kAxis_Z,
    kAxis_W
};

enum Side
{
    kSide_Min,
    kSide_Max
};

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

enum Month
{
    kMonth_Jan,
    kMonth_Feb,
    kMonth_Mar,
    kMonth_Apr,
    kMonth_May,
    kMonth_June,
    kMonth_July,
    kMonth_Aug,
    kMonth_Sept,
    kMonth_Oct,
    kMonth_Nov,
    kMonth_Dec
};

enum DayOfWeek
{
    kDayOfWeek_Sunday,
    kDayOfWeek_Monday,
    kDayOfWeek_Tuesday,
    kDayOfWeek_Wednesday,
    kDayOfWeek_Thursday,
    kDayOfWeek_Friday,
    kDayOfWeek_Saturday
};


//=====================
// Time
//=====================

typedef U64 DenseTime;

struct DateTime
{
    U16 msec;   // [0,999]
    U8 sec;     // [0,60]
    U8 min;     // [0,60]
    U8 hour;    // [0,23]
    U8 day;     // [1,31]
    U8 month;   // [1,12]
    S16 year;   // 1 = 1 CE; 2020 = 2020 CE; 0 = 1 BCE; -100 = 101 BCE; etc.
};

////////////////////////////////
// Symbolic Constant Functions

function OperatingSystem operating_system_from_context(void);
function Compiler compiler_from_context(void);
function Arch arch_from_context(void);

function const char* string_from_operating_system(OperatingSystem os);
function const char* string_from_compiler(Compiler compiler);
function const char* string_from_architecture(Arch arch);
function const char* string_from_month(Month month);
function const char* string_from_day_of_week(DayOfWeek day_of_week);

#endif
