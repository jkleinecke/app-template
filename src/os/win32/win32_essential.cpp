
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
            win32_user_path = str8_from_str16(win32_perm_arena, str16(buffer, cap));
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
// Time
//=======================

function DateTime win32_date_time_from_system_time(SYSTEMTIME *in)
{
    DateTime result = {};
    result.year = in->wYear;
    result.month = (U8)in->wMonth;
    result.day = in->wDay;
    result.hour = in->wHour;
    result.min = in->wMinute;
    result.sec = in->wSecond;
    result.msec = in->wMilliseconds;
    return(result);
}

function SYSTEMTIME win32_system_time_from_date_time(DateTime *in)
{
    SYSTEMTIME result = {};
    result.wYear = in->year;
    result.wMonth = in->month;
    result.wDay = in->day;
    result.wHour = in->hour;
    result.wMinute = in->min;
    result.wSecond = in->sec;
    result.wMilliseconds = in->msec;
    return(result);
}

function DenseTime win32_dense_time_from_file_time(FILETIME *file_time)
{
    SYSTEMTIME system_time = {};
    FileTimeToSystemTime(file_time, &system_time);
    DateTime date_time = win32_date_time_from_system_time(&system_time);
    DenseTime result = dense_time_from_date_time(&date_time);
    return(result);
}

function DateTime os_time_utc_now(void)
{
    SYSTEMTIME system_time = {};
    GetSystemTime(&system_time);
    DateTime result = win32_date_time_from_system_time(&system_time);
    return(result);
}

function DateTime os_time_local_from_utc(DateTime *dt)
{
    SYSTEMTIME utc_sys_time = win32_system_time_from_date_time(dt);
    FILETIME utc_file_time = {};
    SystemTimeToFileTime(&utc_sys_time, &utc_file_time);
    FILETIME local_file_time = {};
    FileTimeToLocalFileTime(&utc_file_time, &local_file_time);
    SYSTEMTIME local_sys_time = {};
    FileTimeToSystemTime(&local_file_time, &local_sys_time);
    DateTime result = win32_date_time_from_system_time(&local_sys_time);
    return(result);
}

function DateTime os_time_utc_from_local(DateTime *dt)
{
    SYSTEMTIME local_sys_time = win32_system_time_from_date_time(dt);
    FILETIME local_file_time = {};
    SystemTimeToFileTime(&local_sys_time, &local_file_time);
    FILETIME utc_file_time = {};
    LocalFileTimeToFileTime(&local_file_time, &utc_file_time);
    SYSTEMTIME utc_sys_time = {};
    FileTimeToSystemTime(&utc_file_time, &utc_sys_time);
    DateTime result = win32_date_time_from_system_time(&utc_sys_time);
    return(result);
}

function U64 os_now_microseconds(void)
{
    U64 result = 0;
    LARGE_INTEGER perf_counter = {};
    if(QueryPerformanceCounter(&perf_counter))
    {
        U64 ticks = ((U64)perf_counter.HighPart << 32) | perf_counter.LowPart;
        result = ticks*Million(1)/win32_ticks_per_second;
    }
    return(result);
}

//=======================
// File Handling
//=======================

function FilePropertyFlags win32_props_from_attribs(DWORD attribs)
{
    FilePropertyFlags result = 0;
    if(attribs & FILE_ATTRIBUTE_DIRECTORY)
    {
        result |= FilePropertyFlag_IsFolder;
    }
    return(result);
}

function DataAccessFlags win32_access_from_attribs(DWORD attribs)
{
    DataAccessFlags result = DataAccessFlag_Read|DataAccessFlag_Execute;
    if(!(attribs & FILE_ATTRIBUTE_READONLY))
    {
        result |= DataAccessFlag_Write;
    }
    return(result);
}

function String8 os_file_read(M_Arena *arena, String8 file_name)
{
    M_ArenaTemp scratch = m_get_scratch(&arena, 1);
    String16 file_name16 = str16_from_str8(scratch.arena, file_name);
    HANDLE file = CreateFileW((WCHAR*)file_name16.str, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    String8 result = {};

    if(file != INVALID_HANDLE_VALUE)
    {
        // get the file size
        DWORD hi_size = 0;
        DWORD lo_size = GetFileSize(file, &hi_size);
        U64 total_size = (((U64)hi_size) << 32) | (U64)lo_size;

        // allocate the read buffer
        M_ArenaTemp restore_point = m_begin_temp(arena);
        U8 *buffer = push_array(arena, U8, total_size);

        // read
        U8 *ptr = buffer;
        U8 *opl = buffer + total_size;
        B32 success = true;
        for(;ptr < opl;)
        {
            U64 total_to_read = (U64)(opl - ptr);
            DWORD to_read = (DWORD)total_to_read;
            if(total_to_read > max_U32)
            {
                to_read = max_U32;
            }
            DWORD actual_read = 0;
            if(!ReadFile(file, ptr, to_read, &actual_read, 0))
            {
                success = false;
                break;
            }
            ptr += actual_read;
        }

        if(success)
        {
            result.str = buffer;
            result.size = total_size;
        }
        else
        {
            m_end_temp(restore_point);
        }

        CloseHandle(file);
    }

    m_release_scratch(scratch);

    return(result);
}

function B32 os_file_write(String8 file_name, String8List data)
{
    M_ArenaTemp scratch = m_get_scratch(0, 0);
    String16 file_name16 = str16_from_str8(scratch.arena, file_name);
    HANDLE file = CreateFileW((WCHAR*)file_name16.str, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    B32 result = false;
    if(file != INVALID_HANDLE_VALUE)
    {
        result = true;

        for(String8Node *node = data.first; node != 0; node = node->next)
        {
            U8 *ptr = node->string.str;
            U8 *opl = ptr + node->string.size;
            for(;ptr < opl;)
            {
                U64 total_to_write = (U64)(opl - ptr);
                DWORD to_write = total_to_write;
                if(total_to_write > max_U32)
                {
                    to_write = max_U32;
                }
                DWORD actual_write = 0;
                if(!WriteFile(file, ptr, to_write, &actual_write, 0))
                {
                    result = false;
                    goto dblbreak;
                }
                ptr += actual_write;
            }
        }
        dblbreak:;

        CloseHandle(file);
    }

    m_release_scratch(scratch);

    return(result);
}

function FileProperties os_file_properties(String8 file_name)
{
    M_ArenaTemp scratch = m_get_scratch(0,0);
    String16 file_name16 = str16_from_str8(scratch.arena, file_name);

    // get attribs and convert
    FileProperties result = {};
    WIN32_FILE_ATTRIBUTE_DATA attribs = {};
    if(GetFileAttributesExW((WCHAR*)file_name16.str, GetFileExInfoStandard, &attribs))
    {
        result.size = ((U64)attribs.nFileSizeHigh << 32) | (U64)attribs.nFileSizeLow;
        result.flags = win32_props_from_attribs(attribs.dwFileAttributes);
        result.create_time = win32_dense_time_from_file_time(&attribs.ftCreationTime);
        result.modify_time = win32_dense_time_from_file_time(&attribs.ftLastWriteTime);
        result.access = win32_access_from_attribs(attribs.dwFileAttributes);
    }

    m_release_scratch(scratch);

    return(result);
}

function B32 os_file_delete(String8 file_name)
{
    M_ArenaTemp scratch = m_get_scratch(0,0);
    String16 file_name16 = str16_from_str8(scratch.arena, file_name);

    B32 result = DeleteFileW((WCHAR*)file_name16.str);
    m_release_scratch(scratch);
    return(result);
}

function B32 os_file_move(String8 src_name, String8 dst_name)
{
    M_ArenaTemp scratch = m_get_scratch(0,0);
    String16 src_name16 = str16_from_str8(scratch.arena, src_name);
    String16 dst_name16 = str16_from_str8(scratch.arena, dst_name);

    B32 result = MoveFileW((WCHAR*)src_name16.str, (WCHAR*)dst_name16.str);
    m_release_scratch(scratch);
    return(result);
}

function B32 os_file_create_directory(String8 path)
{
    M_ArenaTemp scratch = m_get_scratch(0,0);
    String16 path16 = str16_from_str8(scratch.arena, path);

    B32 result = CreateDirectoryW((WCHAR*)path16.str, 0);
    m_release_scratch(scratch);
    return(result);
}

function B32 os_file_delete_directory(String8 path)
{
    M_ArenaTemp scratch = m_get_scratch(0,0);
    String16 path16 = str16_from_str8(scratch.arena, path);

    B32 result = RemoveDirectoryW((WCHAR*)path16.str);
    m_release_scratch(scratch);
    return(result);
}

function OS_FileIter os_file_iter_init(String8 path)
{
    M_ArenaTemp scratch = m_get_scratch(0,0);
    
    String8Node nodes[2];
    String8List list = {};
    str8_list_push_explicit(&list, path, nodes + 0);
    str8_list_push_explicit(&list, str8_lit("\\*"), nodes + 1);
    String8 path_star = str8_join(scratch.arena, &list, 0);
    String16 path16 = str16_from_str8(scratch.arena, path_star);

    OS_FileIter result = {};
    Win32_FileIter *iter = (Win32_FileIter*)&result;
    iter->handle = FindFirstFileW((WCHAR*)path16.str, &iter->find_data);

    m_release_scratch(scratch);
    return(result);
}

function B32 os_file_iter_next(M_Arena *arena, OS_FileIter *iter, String8 *name_out, FileProperties *prop_out)
{
    B32 result = false;

    Win32_FileIter *win32_iter = (Win32_FileIter*)iter;
    if(win32_iter->handle != 0 && win32_iter->handle != INVALID_HANDLE_VALUE)
    {
        for(;!win32_iter->done;)
        {
            // check for . and ..
            WCHAR *file_name = win32_iter->find_data.cFileName;
            B32 is_dot = (file_name[0] == '.' && file_name[1] == 0);
            B32 is_dotdot = (file_name[0] == '.' && file_name[1] == '.' && file_name[2] == 0);

            B32 emit = (!is_dot && !is_dotdot);
            WIN32_FIND_DATAW data = {};
            if(emit)
            {
                MemoryCopyStruct(&data, &win32_iter->find_data);
            }

            if(!FindNextFileW(win32_iter->handle, &win32_iter->find_data))
            {
                win32_iter->done = true;
            }

            if(emit)
            {
                *name_out = str8_from_str16(arena, str16_cstring((U16*)data.cFileName));
                prop_out->size = ((U64)data.nFileSizeHigh << 32) | (U64)data.nFileSizeLow;
                prop_out->flags = win32_props_from_attribs(data.dwFileAttributes);
                prop_out->create_time = win32_dense_time_from_file_time(&data.ftCreationTime);
                prop_out->modify_time = win32_dense_time_from_file_time(&data.ftLastWriteTime);
                prop_out->access = win32_access_from_attribs(data.dwFileAttributes);
                result = true;
                break;
            }
        }
    }

    return(result);
}

function void os_file_iter_end(OS_FileIter *iter)
{
    Win32_FileIter *win32_iter = (Win32_FileIter*)iter;
    if(win32_iter->handle != 0 && win32_iter->handle != INVALID_HANDLE_VALUE)
    {
        FindClose(win32_iter->handle);
    }
}

function String8 os_file_path(M_Arena *arena, OS_SystemPath path)
{
    String8 result = {};

    switch(path)
    {
        case kOS_SystemPath_CurrentDir:
        {
            M_ArenaTemp scratch = m_get_scratch(&arena, 1);
            DWORD cap = 2048;
            U16 *buffer = push_array(scratch.arena, U16, cap);
            DWORD size = GetCurrentDirectoryW(cap, (WCHAR*)buffer);
            if(size >= cap)
            {
                m_end_temp(scratch);
                buffer = push_array(scratch.arena, U16, size+1);
                size = GetCurrentDirectoryW(size+1, (WCHAR*)buffer);
            }
            result = str8_from_str16(arena, str16(buffer, size));
            m_release_scratch(scratch);
        }break;
        case kOS_SystemPath_Bin:
        {
            result = str8_push_copy(arena, win32_binary_path);
        }break;
        case kOS_SystemPath_User:
        {   
            result = str8_push_copy(arena, win32_user_path);
        }break;
        case kOS_SystemPath_Temp:
        {
            result = str8_push_copy(arena, win32_temp_path);
        }break;
    }

    return(result);
}

//=======================
// Threading
//=======================

function void os_sleep_milliseconds(U32 t)
{
    Sleep(t);
}

//=======================
// Libraries
//=======================

function OS_Library os_lib_load(String8 path)
{
    OS_Library result = {};
    M_ArenaTemp scratch = m_get_scratch(0,0);
    String16 path16 = str16_from_str8(scratch.arena, path);
    result.v[0] = (U64)(LoadLibraryW((WCHAR*)path16.str));
    m_release_scratch(scratch);
    return(result);
}

function VoidFunc* os_lib_get_proc(OS_Library lib, char *name)
{
    HMODULE module = (HMODULE)(lib.v[0]);
    VoidFunc *result = (VoidFunc*)(GetProcAddress(module, name));
    return(result);
}

function void os_lib_unload(OS_Library lib)
{
    HMODULE module = (HMODULE)(lib.v[0]);
    FreeLibrary(module);
}

//=======================
// Entropy
//=======================

function void os_get_entropy(void *data, U64 size)
{
    HCRYPTPROV prov = 0;
    CryptAcquireContextW(&prov, 0, 0, PROV_DSS, CRYPT_VERIFYCONTEXT);
    CryptGenRandom(prov, size, (BYTE*)data);
    CryptReleaseContext(prov, 0);
}
