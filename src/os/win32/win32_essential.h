#ifndef WIN32_ESSENTIAL_H
#define WIN32_ESSENTIAL_H

//==================
// Windows Includes
//==================

#undef function
#include <Windows.h>
#define function static

#include <userenv.h>

//==================
// Win32 File Iter
//==================

struct Win32_FileIter
{
    HANDLE handle;
    WIN32_FIND_DATAW find_data;
    B32 done;
};
StaticAssert(sizeof(Win32_FileIter) <= sizeof(OS_FileIter), Win32_FileIter);

//==================
// Win32 init
//==================

function void win32_WinMain_init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

//====================
// Win32 Time Helpers
//====================

function DateTime   win32_date_time_from_system_time(SYSTEMTIME *in);
function SYSTEMTIME win32_system_time_from_date_time(DateTime *in);
function DenseTime  win32_dense_time_from_file_time(FILETIME *file_time);

//====================
// Win32 File Helpers
//====================

function FilePropertyFlags win32_props_from_attribs(DWORD attribs);
function DataAccessFlags win32_access_from_attribs(DWORD attribs);


//====================
// Win32 Threading
//====================

struct Win32Mutex
{
    CRITICAL_SECTION    handle;
};

#endif // WIN32_ESSENTIAL_H
