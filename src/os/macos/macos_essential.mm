
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <copyfile.h>
#include <libproc.h>
#include <time.h>
#include <dlfcn.h>
#include <fcntl.h>

global M_Arena *macos_perm_arena = 0;
global String8 macos_binary_path = {};
global String8 macos_user_path = {};
global String8 macos_temp_path = {};

global String8List macos_cmd_line = {};

//=======================
// Setup 
//=======================

function void os_main_init(int argc, char **argv)
{
    // arena
    macos_perm_arena = m_alloc_arena();

    for(int i = 0; i < argc; ++i)
    {
        String8 arg = str8_cstring((U8*)argv[i]);
        str8_list_push(macos_perm_arena, &macos_cmd_line, arg);
    }

    // binary path
    {
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

function String8 os_file_read(M_Arena *arena, String8 file_name)
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
    return false;
}

function B32 os_file_move(String8 src_name, String8 dst_name)
{
    return false;
}

function B32 os_file_create_directory(String8 path)
{
    return false;
}

function B32 os_file_delete_directory(String8 path)
{
    return false;
}

function OS_FileIter os_file_iter_init(String8 path)
{
    OS_FileIter result = {};
    return(result);
}

function B32 os_file_iter_next(M_Arena *arena, OS_FileIter *iter, String8 *name_out, FileProperties *prop_out)
{
    return false;
}

function void  os_file_iter_end(OS_FileIter *iter)
{

}

function String8 os_file_path(M_Arena *arena, OS_SystemPath path)
{
    String8 result = {};
    return(result);
}

//=======================
// Time
//=======================

function DateTime os_time_utc_now(void)
{
    DateTime result = {};
    return(result);
}

function DateTime os_time_local_from_utc(DateTime *dt)
{
    DateTime result = {};
    return(result);
}

function DateTime os_time_utc_from_local(DateTime *dt)
{
    DateTime result = {};
    return(result);
}

function U64 os_now_microseconds(void)
{
    return 0;
}

//=======================
// Threading
//=======================

function void os_sleep_milliseconds(U32 t)
{

}

//=======================
// Libraries
//=======================

function OS_Library os_lib_load(String8 path)
{
    OS_Library result = {};
    return(result);
}

function VoidFunc*  os_lib_get_proc(OS_Library lib, char *name)
{
    VoidFunc* func = 0;
    return(func);
}

function void os_lib_unload(OS_Library lib)
{

}

//=======================
// Entropy
//=======================

function void os_get_entropy(void *data, U64 size)
{

}
