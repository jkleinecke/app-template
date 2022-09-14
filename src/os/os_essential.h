#ifndef OS_ESSENTIAL_H
#define OS_ESSENTIAL_H

enum OS_SystemPath
{
    kOS_SystemPath_CurrentDir,
    kOS_SystemPath_Bin,
    kOS_SystemPath_User,
    kOS_SystemPath_Temp
};

int app_main(void);

//=======================
// Setup 
//=======================

function void os_main_init(int argc, char **argv);
function String8List os_command_line_args(void);

//=======================
// Memory 
//=======================

function void* os_memory_reserve(U64 size);
function B32   os_memory_commit(void *ptr, U64 size);
function void  os_memory_decommit(void *ptr, U64 size);
function void  os_memory_release(void *ptr, U64 size);

//=======================
// File Handling
//=======================

function String8 os_file_read(M_Arena *arena, String8 file_name);
function B32     os_file_write(String8 file_name, String8List data);

function FileProperties os_file_properties(String8 file_name);

function B32 os_file_delete(String8 file_name);
function B32 os_file_move(String8 src_name, String8 dst_name);
function B32 os_file_create_directory(String8 path);
function B32 os_file_delete_directory(String8 path);

struct OS_FileIter
{
    U8 v[640];
};

function OS_FileIter os_file_iter_init(String8 path);
function B32         os_file_iter_next(M_Arena *arena, OS_FileIter *iter, String8 *name_out, FileProperties *prop_out);
function void        os_file_iter_end(OS_FileIter *iter);

function String8 os_file_path(M_Arena *arena, OS_SystemPath path);

//=======================
// Time
//=======================

function DateTime os_time_utc_now(void);
function DateTime os_time_local_from_utc(DateTime *dt);
function DateTime os_time_utc_from_local(DateTime *dt);

function U64 os_now_microseconds(void);

//=======================
// Threading
//=======================

function void os_sleep_milliseconds(U32 t);

//=======================
// Libraries
//=======================

struct OS_Library{
    U64 v[1];
};

function OS_Library os_lib_load(String8 path);
function VoidFunc*  os_lib_get_proc(OS_Library lib, char *name);
function void       os_lib_unload(OS_Library lib);

//=======================
// Entropy
//=======================

function void os_get_entropy(void *data, U64 size);

#endif
