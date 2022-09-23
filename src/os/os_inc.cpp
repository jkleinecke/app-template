
#include "os_helpers.cpp"

#if OS_WINDOWS
# include "win32/win32_essential.h"
# include "win32/win32_essential.cpp"
#elif OS_MACOS
# include "macos/macos_essential.h"
# include "macos/macos_essential.mm"
#else
# error no backend for os_inc.cpp on this OS
#endif

/***********************************
 * Main entry point abstracted
 ***********************************/
#if OS_WINDOWS
// TODO(james): Use WinMainCRTStartup() instead to completely omit the linkage of the C Runtime library
// extern "C" int __stdcall WinMainCRTStartup()
// {
//      SetDefaultFPBehavior(); // control floating point handling
//      HINSTANCE hInstance = GetModuleHandle(0);
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    win32_WinMain_init(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
#else
int main(int argc, char** argv)
{
    os_main_init(argc, argv);
#endif

    int result = app_main();

    return(result);
}
