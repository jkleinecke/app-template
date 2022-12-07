
#include "win32_window_opengl.h"

// #include "imgui/backends/imgui_impl_win32.h"
// #include "imgui/backends/imgui_impl_opengl3.h"

global wgl_create_context_attribs_arb* wglCreateContextAttribsARB;
global wgl_choose_pixel_format_arb* wglChoosePixelFormatARB;
global wgl_swap_interval_ext* wglSwapIntervalEXT;
global wgl_get_extensions_string_ext* wglGetExtensionsStringEXT;
global PFN_glGenerateMipmap* glGenerateMipmap;

global int Win32OpenGLAttribs[] =
{
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#if PROJECTSUPER_INTERNAL
    |WGL_CONTEXT_DEBUG_BIT_ARB
#endif
    ,
#if 0
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
#else
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#endif
    0,
};

function void Win32SetPixelFormat(HDC hDeviceContext)
{
    int suggestedPixelFormatIndex = 0;
    GLuint extendedPixelFormatsCount = 0;
    if(wglChoosePixelFormatARB)
    {
        int intAttribsList[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, // 0
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE, // 1
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE, // 2
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // 3
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, // 4
            WGL_COLOR_BITS_ARB, 32, // 5
            WGL_ALPHA_BITS_ARB, 8, // 6
            WGL_DEPTH_BITS_ARB, 24, // 7
            WGL_STENCIL_BITS_ARB, 8, // 8
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // 9
            WGL_SAMPLES_ARB, 4, // 10
            0
        };
        
        wglChoosePixelFormatARB(hDeviceContext, intAttribsList, 0, 1, &suggestedPixelFormatIndex, &extendedPixelFormatsCount);
    }
    
    if(!extendedPixelFormatsCount)
    {
        PIXELFORMATDESCRIPTOR fakePFD = {};
        fakePFD.nSize = sizeof(fakePFD);
        fakePFD.nVersion = 1;
        fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        fakePFD.iPixelType = PFD_TYPE_RGBA;
        fakePFD.cColorBits = 32;
        fakePFD.cAlphaBits = 8;
        fakePFD.cDepthBits = 24;
        
        suggestedPixelFormatIndex = ChoosePixelFormat(hDeviceContext, &fakePFD);
    }
    
    PIXELFORMATDESCRIPTOR suggestedPixelFormat;
    DescribePixelFormat(hDeviceContext, suggestedPixelFormatIndex, sizeof(suggestedPixelFormat), &suggestedPixelFormat);
    SetPixelFormat(hDeviceContext, suggestedPixelFormatIndex, &suggestedPixelFormat);
}

function HGLRC Win32InitOpenGL(Win32Window* w32window)
{
    WNDCLASSEX loaderClass = {};
    loaderClass.cbSize = sizeof(loaderClass);
    loaderClass.lpfnWndProc = DefWindowProc;
    loaderClass.hInstance = win32_main_instance;
    loaderClass.lpszClassName = "GlLoader";
    RegisterClassEx(&loaderClass);

    // We first have to create a fake window and then
    // initialize OpenGL on that window to get the
    // wglChoosePixelFormatARB extension function.
    HWND hFakeWnd = CreateWindowExA(0,
                                    loaderClass.lpszClassName, "Loader", 
                                 0,
                                 CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
                                 0,0,
                                 loaderClass.hInstance, 0
                                 );
    
    HDC hFakeDC = GetDC(hFakeWnd);
    Win32SetPixelFormat(hFakeDC);
    
    HGLRC hFakeRC = wglCreateContext(hFakeDC);
    Assert(hFakeRC);  // TODO(james): log out error
    
    if(wglMakeCurrent(hFakeDC, hFakeRC))
    {
        // Now we can access the extended pixel formats with the full set of 
        // functionality available.  Petty Windows was trying to hide this stuff
        // from us because it got mad...
        wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb*)wglGetProcAddress("wglChoosePixelFormatARB");
        wglCreateContextAttribsARB = (wgl_create_context_attribs_arb*)wglGetProcAddress("wglCreateContextAttribsARB");
        wglSwapIntervalEXT = (wgl_swap_interval_ext*)wglGetProcAddress("wglSwapIntervalEXT");
        wglGetExtensionsStringEXT = (wgl_get_extensions_string_ext*)wglGetProcAddress("wglGetExtensionsStringEXT");
        
        // TODO(james): use wglGetExtensionsStringEXT to gather a basic set of capabilities 
        
        wglMakeCurrent(0, 0);
    }
    
    wglDeleteContext(hFakeRC);
    ReleaseDC(hFakeWnd, hFakeDC);
    DestroyWindow(hFakeWnd);
                                 
    Win32SetPixelFormat(w32window->hDC);
    
    HGLRC glc = wglCreateContextAttribsARB(w32window->hDC, 0, Win32OpenGLAttribs);
    Assert(glc); // TODO(james): more better verification
    w32window->hGLC = glc;

    glGenerateMipmap = (PFN_glGenerateMipmap*)wglGetProcAddress("glGenerateMipmap");

    return(glc);
}

LRESULT CALLBACK AppWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    Win32Window* w32window = 0;

    if(uMsg == WM_CREATE)
    {
        LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)lpCreate->lpCreateParams);
        w32window = (Win32Window*)lpCreate->lpCreateParams;
    }
    else
    {
        LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        w32window = (Win32Window*)ptr;
    }

    switch(uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            result = TRUE;
            break;
        default:
            // result = ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
            // if(!result)
            // {
                result = DefWindowProc(hWnd, uMsg, wParam, lParam);
            // }
            break;
    }

    return(result);
}

function void Win32RegisterInitialWindow(void)
{
    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.lpfnWndProc = AppWinProc;
    wndClass.hInstance = win32_main_instance;
    wndClass.lpszClassName = "AppWindow";
    RegisterClassEx(&wndClass);

    // IMGUI_CHECKVERSION();
    // ImGui::CreateContext();
    // ImGui_ImplWin32_EnableDpiAwareness();   // This may not be necessary, but it's a quick win

    // ImGui::StyleColorsDark();
    // // ImGui::StyleColorsLight();
}

function OSWindow os_window_create(String8 title, U32 width, U32 height)
{
    local B32 registered = false;
    if(!registered)
    {
        Win32RegisterInitialWindow();
        registered = true;
    }

    MemArena* arena = mem_alloc_arena();    // TODO: specify smaller arena here?

    Win32Window* w32window = push_struct(arena, Win32Window);
    w32window->arena = arena;
    w32window->hWnd = CreateWindow("AppWindow", 
        (LPCSTR)title.str,
        WS_OVERLAPPEDWINDOW,
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        win32_main_instance,  // Instance handle
        w32window        // Additional application data
        );

    w32window->hDC = GetDC(w32window->hWnd);

    // actually make the window visible
    ShowWindow(w32window->hWnd, SW_SHOW);
    UpdateWindow(w32window->hWnd);

    w32window->hGLC = Win32InitOpenGL(w32window);
    wglMakeCurrent(w32window->hDC, w32window->hGLC);
    wglSwapIntervalEXT(1);  // v-sync on

    // ImGui_ImplWin32_Init(w32window->hWnd);

    // if(!registered)
    // {
    //     // may need to be #version 150 for APPLE
    //     ImGui_ImplOpenGL3_Init();
    //     registered = true;
    // }

    return(w32window);
}

function void os_window_destroy(OSWindow window)
{
    Win32Window* w32window = (Win32Window*)window;

    // // TODO(james): Only do this on the main app window
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplWin32_Shutdown();
    // ImGui::DestroyContext();

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(w32window->hWnd, w32window->hDC);
    DestroyWindow(w32window->hWnd);

    mem_arena_release(w32window->arena);
}

function void os_window_needs_refresh(OSWindow window)
{
    Win32Window* w32window = (Win32Window*)window;

    InvalidateRect(w32window->hWnd, NULL, FALSE);
}

function void os_window_render_begin(OSWindow window)
{
    Win32Window* w32window = (Win32Window*)window;
    wglMakeCurrent(w32window->hDC, w32window->hGLC);

    // ImGui_ImplOpenGL3_NewFrame();
    // ImGui_ImplWin32_NewFrame();
    // ImGui::NewFrame();
}

function void os_window_render_end(OSWindow window)
{
    Win32Window* w32window = (Win32Window*)window;

    // TODO: Verify that another window render wasn't started before this one ended

    // ImGui::Render();

    RECT rc;
    GetClientRect(w32window->hWnd, &rc);
    // TODO: Should we do this here?
    glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);

    glClearColor(0.129f, 0.586f, 0.949f, 1.0f); // rgb(33,150,243) sky blue?
    glClear(GL_COLOR_BUFFER_BIT);

    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SwapBuffers(w32window->hDC);
    // wglMakeCurrent(NULL,NULL);      // should we bother with this?
}

function OSEventType os_poll_window_events(void)
{
    B32 running = true;
    MSG msg = {};

    //if(GetMessage(&msg, 0, 0, 0))
    
    BOOL bResult = GetMessage(&msg, 0, 0, 0);

    if(bResult > 0)
    // if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        switch(msg.message)
        {
            case WM_QUIT:
                running = false;
                // fall through
            default:
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                break;
        }
    }
    else
    {
        // bResult < 0 is an error
        // bResult == 0 is WM_QUIT
        running = false;
    }

    return running ? kOSEventType_NeedsRender : kOSEventType_Quit;
}
