
global U64 win32_ticks_per_second = 1;

global M_Arena *win32_perm_arena = 0;
global String8 win32_binary_path = {};
global String8 win32_user_path = {};
global String8 win32_temp_path = {};

global String8List win32_cmd_line = {};

//==================
// Init
//==================

function void os_main_init(int argc, char **argv)
{
    // precision time setup
    LARGE_INTEGER perf_freq = {};
    if(QueryPerformanceFrequency(&perf_freq))
    {
        win32_ticks_per_second = ((U64)perf_freq.HighPart << 32 | perf_freq.LowPart);
    }
    // tell os we want 1ms sleep times...
    timeBeginPeriod(1);

    // arena
    win32_perm_arena = m_alloc_arena();

    // command line args
    for(int i = 0; i < argc; ++i)
    {
        String8 arg = str8_cstring((U8*)argv[i]);
        str8_list_push(win32_perm_arena, &win32_cmd_line, arg);
    }

    // paths
    M_ArenaTemp scratch = m_get_scratch(0,0);

    // binary path
    {
        DWORD cap = 2048;
        U16* buffer = 0;
        DWORD size = 0;
        for(U64 r = 0; r < 4; ++r, cap *= 4)
        {
            U16 *try_buffer = push_array(scratch.arena, U16, cap);
            DWORD try_size = GetModuleFileNameW(0, (WCHAR*)try_buffer, cap);
            if(try_size == cap && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                m_end_temp(scratch);
            }
            else
            {
                buffer = try_buffer;
                size = try_size;
                break;
            }
        }

        String8 full_path = str8_from_str16(scratch.arena, str16(buffer,size));
        String8 bin_path = str8_chop_last_slash(full_path);
        win32_binary_path = str8_push_copy(win32_perm_arena, bin_path);
    }

    // user path
    {
        HANDLE token = GetCurrentProcessToken();
        DWORD cap = 2048;
        U16 *buffer = push_array(scratch.arena, U16, cap);
        if(!GetUserProfileDirectoryW(token, (WCHAR*)buffer, &cap))
        {
            m_end_temp(scratch);
            buffer = push_array(scratch.arena, U16, cap+1);
            if(!GetUserProfileDirectoryW(token, (WCHAR*)buffer, &cap))
            {
                buffer = 0;
            }
        }

        if(buffer != 0)
        {
            win32_user_path = str8_from_str16(win32_perm_arena, str16_cstring(buffer));
        }
    }

    // temp path
    {
        DWORD cap = 2048;
        U16 *buffer = push_array(scratch.arena, U16, cap);
        DWORD size = GetTempPathW(cap, (WCHAR*)buffer);
        if(size >= cap)
        {
            m_end_temp(scratch);
            buffer = push_array(scratch.arena, U16, size + 1);
            size = GetTempPathW(size+1, (WCHAR*)buffer);
        }

        // This win32 api includes the trailing slash, so lets remove it..
        win32_temp_path = str8_from_str16(win32_perm_arena, str16(buffer, size - 1));
    }

    m_release_scratch(scratch);
}

function void win32_WinMain_init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    int argc = __argc;
    char **argv = __argv;
    os_main_init(argc, argv);
}

function String8List os_command_line_args(void)
{
    String8List result = win32_cmd_line;
    return(result);
}

//==================
// Memory
//==================

function void* os_memory_reserve(U64 size)
{
    void* result = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
    return(result);
}

function B32 os_memory_commit(void *ptr, U64 size)
{
    B32 result = (VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != 0);
    return(result);
}

function void os_memory_decommit(void *ptr, U64 size)
{
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

function void os_memory_release(void *ptr, U64 size)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

//=======================
// File Handling
//=======================


//=======================
// Threading
//=======================


//=======================
// Libraries
//=======================
