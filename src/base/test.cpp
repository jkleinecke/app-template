#include "base.cpp"
#include "base_default_memory.h"
#include "base_memory.h"
#include "base_memory.cpp"
#include "stdio.h"

#define TEST_PRINT(func) printf(Stringify(func)": %s\n", func)

int main(int argc, char** argv)
{
    printf("Operation System: %s\n", string_from_operating_system(operating_system_from_context()));
    printf("Compiler: %s %s\n", string_from_compiler(compiler_from_context()), compiler_version());
    printf("Architecture: %s\n", string_from_architecture(arch_from_context()));

    TEST_PRINT(string_from_operating_system(kOS_Windows));
    TEST_PRINT(string_from_operating_system(kOS_MacOS));
    TEST_PRINT(string_from_operating_system(kOS_Linux));

    TEST_PRINT(string_from_compiler(kCompiler_CL));
    TEST_PRINT(string_from_compiler(kCompiler_Clang));
    TEST_PRINT(string_from_compiler(kCompiler_GCC));

    TEST_PRINT(string_from_architecture(kArch_x86));
    TEST_PRINT(string_from_architecture(kArch_x64));
    TEST_PRINT(string_from_architecture(kArch_ARM));
    TEST_PRINT(string_from_architecture(kArch_ARM64));

    return 0;
}
