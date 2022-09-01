
function OperatingSystem operating_system_from_context(void)
{
#if OS_WINDOWS
    return kOS_Windows;
#elif OS_MACOS
    return kOS_MacOS;
#elif OS_LINUX
    return kOS_Linux;
#endif
}

function Compiler compiler_from_context(void)
{
#if COMPILER_CL
    return kCompiler_CL;
#elif COMPILER_CLANG
    return kCompiler_Clang;
#elif COMPILER_GCC
    return kCompiler_GCC;
#endif
}

function Arch arch_from_context(void)
{
#if ARCH_X86
    return kArch_x86;
#elif ARCH_X64
    return kArch_x64;
#elif ARCH_ARM
    return kArch_ARM;
#elif ARCH_ARM64
    return kArch_ARM64;
#endif
}



function const char* string_from_operating_system(OperatingSystem os)
{
    switch(os)
    {
        case kOS_Null: return "Null";
        case kOS_Windows: return "Windows";
        case kOS_MacOS: return "MacOS";
        case kOS_Linux: return "Linux";
    }

    return "";
}

function const char* string_from_compiler(Compiler compiler)
{
    switch(compiler)
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

function const char* compiler_version()
{
    return COMPILER_VERSION;
}

function const char* string_from_architecture(Arch arch)
{
    switch(arch)
    {
        case kArch_Null: return "NULL";
        case kArch_x86: return "x86";
        case kArch_x64: return "x64";
        case kArch_ARM: return "Arm";
        case kArch_ARM64: return "Arm64";
    }

    return "";
}

function const char* string_from_month(Month month)
{
    switch(month)
    {
        case kMonth_Jan: return "January";
        case kMonth_Feb: return "February";
        case kMonth_Mar: return "March";
        case kMonth_Apr: return "April";
        case kMonth_May: return "May";
        case kMonth_June: return "June";
        case kMonth_July: return "July";
        case kMonth_Aug: return "August";
        case kMonth_Sept: return "September";
        case kMonth_Oct: return "October";
        case kMonth_Nov: return "November";
        case kMonth_Dec: return "December";
    }

    return "";
}

function const char* string_from_day_of_week(DayOfWeek day_of_week)
{
    switch(day_of_week)
    {
        case kDayOfWeek_Sunday: return "Sunday";
        case kDayOfWeek_Monday: return "Monday";
        case kDayOfWeek_Tuesday: return "Tuesday";
        case kDayOfWeek_Wednesday: return "Wednesday";
        case kDayOfWeek_Thursday: return "Thursday";
        case kDayOfWeek_Friday: return "Friday";
        case kDayOfWeek_Saturday: return "Saturday";
    }

    return "";
}
