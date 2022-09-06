#include "../base/base_inc.h"
#include "os_essential.h"

#include "../base/base_inc.cpp"
#include "os_inc.cpp"

#define TEST_PRINT(func) printf(Stringify(func)": %s\n", func)

struct TestData
{
    S32 a;
    S32 b;
};

int main(int argc, char **argv)
{
    os_main_init(argc, argv);

    TEST_PRINT(win32_binary_path.str);
    TEST_PRINT(win32_user_path.str);
    TEST_PRINT(win32_temp_path.str);

    for(String8Node *node = win32_cmd_line.first; node != 0; node = node->next)
    {
        printf("Command Line Arg: %s\n", node->string.str);
    }

    String8 filedata = str8_lit("This is some text to write to a file");

    B32 result = os_file_write(str8_lit("test.txt"), filedata);
    printf("File Write Result: %d\n", result);

    M_Arena* arena = m_alloc_arena();

    String8 readdata = os_file_read(arena, str8_lit("test.txt"));
    printf("File Read Result: %s\n", readdata.str);
    result = str8_match(filedata, readdata, 0);
    printf("Read data match result: %d\n", result);

    m_arena_release(arena);

    return 0;
}
