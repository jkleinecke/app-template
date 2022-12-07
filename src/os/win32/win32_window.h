#ifndef WIN32_WINDOW_H_
#define WIN32_WINDOW_H_

struct Win32Window
{
    MemArena* arena;
    HWND    hWnd;
    HDC     hDC;
    HGLRC   hGLC;
};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // WIN32_WINDOW_H_
