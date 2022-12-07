

// TODO(james): Use WinMainCRTStartup() instead to completely omit the linkage of the C Runtime library
// extern "C" int __stdcall WinMainCRTStartup()
// {
//      SetDefaultFPBehavior(); // control floating point handling
//      HINSTANCE hInstance = GetModuleHandle(0);
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    win32_WinMain_init(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

    int result = app_main();

    return(result);
}
