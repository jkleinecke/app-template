#include "base.cpp"
#include "base_default_memory.h"
#include "base_memory.h"
#include "base_memory.cpp"
#include "base_string.h"
#include "stdio.h"
#include "base_string.cpp"

#define TEST_PRINT(func) printf(Stringify(func)": %s\n", func)

struct TestData
{
    S32 a;
    S32 b;
};

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

    printf("Memory Tests\n");
    M_Arena* arena = m_alloc_arena();
    TestData* ptr = push_struct(arena,TestData);

    printf("pos: %llu, commit_pos: %llu, cap: %llu\n", arena->pos, arena->commit_pos, arena->cap);

    int* array_ints = push_array_zero(arena,int,10);
    for(int i = 0; i < 10; i++)
    {
        printf("array %d: %d\n", i, array_ints[i]);
    }
    printf("pos: %llu, commit_pos: %llu, cap: %llu\n", arena->pos, arena->commit_pos, arena->cap);

    return 0;
}
