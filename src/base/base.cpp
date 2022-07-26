#include "base.h"

OperatingSystem os_from_context()
{
    return OS_ENUM;
}

Compiler compiler_from_context()
{
    return COMPILER_ENUM;
}

Arch arch_from_context()
{
    return ARCH_ENUM;
}

const char* os_name(OperatingSystem eOS = OS_ENUM)
{
    switch(eOS)
    {
        case kOS_Null: return "Null";
        case kOS_Windows: return "Windows";
        case kOS_MacOS: return "MacOS";
        case kOS_Linux: return "Linux";
    }

    return "";
}

const char* compiler_name(Compiler eCompiler = COMPILER_ENUM)
{
    switch(eCompiler)
    {
        case kCompiler_Null: return "Null";
        case kCompiler_CL: return "Visual C++";
    #ifdef __apple_build_version__
        case kCompiler_Clang: return "Apple Clang";
    #else
        case kCompiler_Clang: return "Clang";
    #endif
        case kCompiler_GCC: return "GCC";
    }

    return "";
}

const char* compiler_version()
{
    return COMPILER_VERSION;
}

const char* arch_name(Arch eArch = ARCH_ENUM)
{
    switch(eArch)
    {
        case kArch_Null: return "NULL";
        case kArch_x86: return "x86";
        case kArch_x64: return "x64";
        case kArch_ARM: return "Arm";
        case kArch_ARM64: return "Arm64";
    }

    return "";
}
