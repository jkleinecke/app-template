


#include <sys/mman.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <copyfile.h>
#include <libproc.h>
#include <time.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <mach/mach_time.h>

global NSApplication* macos_application = 0;
global MacosAppDelegate* macos_appdelegate = 0;

global B32 macos_didquit = false;
global MemArena *macos_perMemArena = 0;
global String8 macos_binary_path = {};
global String8 macos_user_path = {};
global String8 macos_temp_path = {};

global String8List macos_cmd_line = {};


NSString* nsstring_from_str8(MemArena* arena, String8 str)
{
    Assert(str.size != 0);      // Cannot create NSString with empty String8
    String16 str16 = str16_from_str8(arena, str);

    NSString* nsstr = [NSString stringWithCharacters: str16.str length: str16.size];
    return(nsstr);
}

String8 str8_from_nsstring(MemArena* arena, NSString* nsstr)
{
    U32 max_length = [nsstr maximumLengthOfBytesUsingEncoding: NSUTF8StringEncoding];
    U8* bytes = push_array(arena, U8, max_length);

    String8 result = {};
    result.str = bytes;
    result.size = max_length;

    BOOL success = [nsstr getCString: (char*)bytes maxLength: max_length encoding: NSUTF8StringEncoding];
    Assert(success);
    
    return(result);
}

//=======================
// Setup 
//=======================e

function void os_main_init(int argc, char **argv)
{
    macos_application = [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];

    // Set the app delegate
    macos_appdelegate = [[MacosAppDelegate alloc] init];

    [NSApp finishLaunching];

    // arena
    macos_perMemArena = mem_alloc_arena();

    for(int i = 0; i < argc; ++i)
    {
        String8 arg = str8_cstring((U8*)argv[i]);
        str8_list_push(macos_perMemArena, &macos_cmd_line, arg);
    }

    // binary path
    {
        // [[NSBundle mainBundle] bundlePath]];
        macos_binary_path = str8_lit("TODO");
    }

    // user path
    {
        macos_user_path = str8_lit("TODO");
    }

    // temp path
    {
        macos_temp_path = str8_lit("TODO");
    }
}

function String8List os_command_line_args(void)
{
    String8List result = macos_cmd_line;
    return(result);
}

//=======================
// Memory 
//=======================

function void* os_memory_reserve(U64 size)
{
    void* ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    return(ptr);
}

function B32   os_memory_commit(void *ptr, U64 size)
{
    // mmap doesn't require an explicit commit of memory
    // the OS will commit the pages as they are touched according
    // to the documentation
    return true;
}

function void  os_memory_decommit(void *ptr, U64 size)
{
    // do nothing since there is no explicit commit
    // there cannot be an explicit decommit
}

function void  os_memory_release(void *ptr, U64 size)
{
    if(munmap(ptr, size) != 0)
    {
        Assert(false);
    }
}

//=======================
// File Handling
//=======================

function String8 os_file_read(MemArena *arena, String8 file_name)
{
    String8 result = {};

    struct stat fileStats;
	int r = stat((const char*)file_name.str, &fileStats);
    Assert(r == 0);

    int fd = open((const char*)file_name.str, O_RDONLY, 0600);

    if(fd > 0)
    {
        result.str = push_array(arena, U8, fileStats.st_size);

        U64 bytes_read = read(fd, result.str, fileStats.st_size);
        Assert(bytes_read == (U64)fileStats.st_size);

        close(fd);
    }
    else
    {
        Assert(false);
    }

    return result;
}

function B32     os_file_write(String8 file_name, String8List data)
{
    B32 success = false;
    int fd = open((const char*)file_name.str, O_CREAT|O_WRONLY, 0600);

    if(fd > 0)
    {
        success = true;
        for(String8Node* node = data.first; node; node = node->next)
        {
            U64 written = write(fd, node->string.str, node->string.size);
            success = written == node->string.size;
        }

        close(fd);
    }
    return(success);
}

function FileProperties os_file_properties(String8 file_name)
{
    struct stat fileStats;
	int r = stat((const char*)file_name.str, &fileStats);
    Assert(r == 0);

    FileProperties result = {};
    result.size = (U64)fileStats.st_size;
    result.flags = fileStats.st_mode & S_IFDIR ? FilePropertyFlag_IsFolder : 0;
    // TODO: Implement the create/modify time conversion
    NotImplemented;
    result.create_time = 0;
    result.modify_time = 0;

    if(fileStats.st_mode & S_IRUSR)
    {
        result.access |= DataAccessFlag_Read;
    }
    if(fileStats.st_mode & S_IWUSR)
    {
        result.access |= DataAccessFlag_Write;
    }
    if(fileStats.st_mode & S_IXUSR)
    {
        result.access |= DataAccessFlag_Execute;
    }

    return result;
}

function B32 os_file_delete(String8 file_name)
{
    NotImplemented;
    return false;
}

function B32 os_file_move(String8 src_name, String8 dst_name)
{
    NotImplemented;
    return false;
}

function B32 os_file_create_directory(String8 path)
{
    NotImplemented;
    return false;
}

function B32 os_file_delete_directory(String8 path)
{
    NotImplemented;
    return false;
}

function OS_FileIter os_file_iter_init(String8 path)
{
    NotImplemented;
    OS_FileIter result = {};
    return(result);
}

function B32 os_file_iter_next(MemArena *arena, OS_FileIter *iter, String8 *name_out, FileProperties *prop_out)
{
    NotImplemented;
    return false;
}

function void  os_file_iter_end(OS_FileIter *iter)
{
    NotImplemented;
}

function String8 os_file_path(MemArena *arena, OS_SystemPath path)
{
    String8 result = {};

    switch(path)
    {
        case kOS_SystemPath_CurrentDir:
        {
            // NSString *dir = [[NSFileManager defaultManager] currentDirectoryPath];

            // MemArenaTemp scratch = mem_get_scratch(&arena, 1);
            // DWORD cap = 2048;
            // U16 *buffer = push_array(scratch.arena, U16, cap);
            // DWORD size = GetCurrentDirectoryW(cap, (WCHAR*)buffer);
            // if(size >= cap)
            // {
            //     mem_end_temp(scratch);
            //     buffer = push_array(scratch.arena, U16, size+1);
            //     size = GetCurrentDirectoryW(size+1, (WCHAR*)buffer);
            // }
            // result = str8_from_str16(arena, str16(buffer, size));
            // mem_release_scratch(scratch);
        }break;
        case kOS_SystemPath_Bin:
        {
            result = str8_push_copy(arena, macos_binary_path);
        }break;
        case kOS_SystemPath_User:
        {   
            result = str8_push_copy(arena, macos_user_path);
        }break;
        case kOS_SystemPath_Temp:
        {
            result = str8_push_copy(arena, macos_temp_path);
        }break;
    }

    return(result);
}

//=======================
// Time
//=======================

function DateTime macos_datetime_from_timeval(struct timeval* time)
{
    struct tm* timeparts = gmtime(&time->tv_sec);

    DateTime result = {};
    result.msec = time->tv_usec/1000;
    result.sec = timeparts->tm_sec;
    result.min = timeparts->tm_min;
    result.hour = timeparts->tm_hour;
    result.day = timeparts->tm_mday;
    result.month = timeparts->tm_mon;
    result.year = timeparts->tm_year;

    return(result);
}

function struct tm macos_tm_from_datetime(DateTime *dt)
{
    struct tm timeparts = {};
    timeparts.tm_sec = dt->sec;
    timeparts.tm_min = dt->min;
    timeparts.tm_hour = dt->hour;
    timeparts.tm_mday = dt->day;
    timeparts.tm_mon = dt->month;
    timeparts.tm_year = dt->year;

    return(timeparts);
}

function DateTime os_time_utc_now(void)
{
    struct timeval time;
    int failed = gettimeofday(&time, 0);
    Assert(!failed);

    DateTime result = macos_datetime_from_timeval(&time);
    return(result);
}

function DateTime os_time_local_from_utc(DateTime *dt)
{
    struct tm timeparts = macos_tm_from_datetime(dt);
    struct timeval time = {};
    time.tv_sec = mktime(&timeparts);
    time.tv_usec = dt->msec*1000;

    DateTime result = macos_datetime_from_timeval(&time);
    return(result);
}

function DateTime os_time_utc_from_local(DateTime *dt)
{
    struct tm timeparts = macos_tm_from_datetime(dt);
    struct timeval time = {};
    time.tv_sec = timegm(&timeparts);
    time.tv_usec = dt->msec*1000;

    DateTime result = macos_datetime_from_timeval(&time);
    return(result);
}

function U64 os_now_microseconds(void)
{
    U64 result = mach_absolute_time();

    return(result);
}

//=======================
// Threading
//=======================

function void os_sleep_milliseconds(U32 t)
{
    int failed = usleep(t*1000);
    Assert(!failed);
}

//=======================
// Libraries
//=======================

function OS_Library os_lib_load(String8 path)
{
    NotImplemented;
    OS_Library result = {};
    return(result);
}

function VoidFunc*  os_lib_get_proc(OS_Library lib, char *name)
{
    NotImplemented;
    VoidFunc* func = 0;
    return(func);
}

function void os_lib_unload(OS_Library lib)
{
    NotImplemented;
}

//=======================
// Entropy
//=======================

function void os_get_entropy(void *data, U64 size)
{
    NotImplemented;
}
