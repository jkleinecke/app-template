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

int app_main(void)
{
    M_ArenaTemp scratch = m_get_scratch(0,0);

    TEST_PRINT(os_file_path(scratch.arena, kOS_SystemPath_CurrentDir).str);
    TEST_PRINT(os_file_path(scratch.arena, kOS_SystemPath_Bin).str);
    TEST_PRINT(os_file_path(scratch.arena, kOS_SystemPath_Temp).str);
    TEST_PRINT(os_file_path(scratch.arena, kOS_SystemPath_User).str);

    U64 now = os_now_microseconds();
    printf("microseconds: %llu\n", now);

    String8List listCmdArgs = os_command_line_args();
    for(String8Node *node = listCmdArgs.first; node != 0; node = node->next)
    {
        printf("Command Line Arg: %s\n", node->string.str);
    }

    String8 filedata = str8_lit("This is some text to write to a file");

    B32 result = os_file_write(str8_lit("test.txt"), filedata);
    printf("File Write Result: %d\n", result);

    {
        M_Arena* arena = m_alloc_arena();

        String8 readdata = os_file_read(arena, str8_lit("test.txt"));
        printf("File Read Result: %s\n", readdata.str);
        result = str8_match(filedata, readdata, 0);
        printf("Read data match result: %d\n", result);

        m_arena_release(arena);
    }

    m_release_scratch(scratch);


    now = os_now_microseconds();
    printf("microseconds: %llu\n", now);

    return 0;
}
