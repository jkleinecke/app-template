#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.cpp"
#include "os/os_inc.cpp"

#include <gl/GL.h>

// Some Windows OpenGL specifics

#define WGL_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_FLAGS_ARB             0x2094

#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_TYPE_RGBA_ARB                 0x202B

#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042

#define GLAPI WINAPI

typedef HGLRC WINAPI wgl_create_context_attribs_arb(HDC hDC, HGLRC hShareContext,
                                                    const int *attribList);

typedef BOOL WINAPI wgl_get_pixel_format_attrib_iv_arb(HDC hdc,
                                                       int iPixelFormat,
                                                       int iLayerPlane,
                                                       UINT nAttributes,
                                                       const int *piAttributes,
                                                       int *piValues);

typedef BOOL WINAPI wgl_get_pixel_format_attrib_fv_arb(HDC hdc,
                                                       int iPixelFormat,
                                                       int iLayerPlane,
                                                       UINT nAttributes,
                                                       const int *piAttributes,
                                                       FLOAT *pfValues);

typedef BOOL WINAPI wgl_choose_pixel_format_arb(HDC hdc,
                                                const int *piAttribIList,
                                                const FLOAT *pfAttribFList,
                                                UINT nMaxFormats,
                                                int *piFormats,
                                                UINT *nNumFormats);

typedef BOOL WINAPI wgl_swap_interval_ext(int interval);
typedef const char* WINAPI wgl_get_extensions_string_ext(void);

global wgl_create_context_attribs_arb* wglCreateContextAttribsARB;
global wgl_choose_pixel_format_arb* wglChoosePixelFormatARB;
global wgl_swap_interval_ext* wglSwapIntervalEXT;
global wgl_get_extensions_string_ext* wglGetExtensionsStringEXT;

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

function HGLRC Win32InitOpenGL(HDC hDeviceContext)
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
                                 
    Win32SetPixelFormat(hDeviceContext);
    
    HGLRC glc = wglCreateContextAttribsARB(hDeviceContext, 0, Win32OpenGLAttribs);
    Assert(glc); // TODO(james): more better verification
    
    wglMakeCurrent(hDeviceContext, glc);

    // TODO(james): Load all opengl extensions here
    // #define LoadOpenGLFunction(name)  gl_instance.##name = (type_##name *)wglGetProcAddress(#name)

    // LoadOpenGLFunction(glGenBuffers);
    // LoadOpenGLFunction(glBindBuffer);
    // LoadOpenGLFunction(glBufferData);
    // LoadOpenGLFunction(glVertexAttribPointer);
    // LoadOpenGLFunction(glGetAttribLocation);
    // LoadOpenGLFunction(glGetUniformLocation);
    // LoadOpenGLFunction(glCreateShader);
    // LoadOpenGLFunction(glDeleteShader);
    // LoadOpenGLFunction(glShaderSource);
    // LoadOpenGLFunction(glCompileShader);
    // LoadOpenGLFunction(glCreateProgram);
    // LoadOpenGLFunction(glAttachShader);
    // LoadOpenGLFunction(glDetachShader);
    // LoadOpenGLFunction(glLinkProgram);
    // LoadOpenGLFunction(glValidateProgram);
    // LoadOpenGLFunction(glUseProgram);
    // LoadOpenGLFunction(glGetProgramiv);
    // LoadOpenGLFunction(glGetShaderiv);
    // LoadOpenGLFunction(glGetShaderInfoLog);
    // LoadOpenGLFunction(glGetProgramInfoLog);
    // LoadOpenGLFunction(glGenVertexArrays);
    // LoadOpenGLFunction(glBindVertexArray);
    // LoadOpenGLFunction(glEnableVertexAttribArray);

    // init_test_gl(gl_instance, windowContext.graphics);
    return(glc);
}

//

LRESULT CALLBACK AppWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch(uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            result = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return(result);
}

int app_main(void)
{
    printf("Hello World!\n");

    String8List cmd_args = os_command_line_args();
    for(String8Node* node = cmd_args.first; node; node = node->next)
    {
        printf("Arg: %s\n", node->string.str);
    }

    WNDCLASSEX wndClass = {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.lpfnWndProc = AppWinProc;
    wndClass.hInstance = win32_main_instance;
    wndClass.lpszClassName = "AppWindow";
    RegisterClassEx(&wndClass);

    HWND mainWindow = CreateWindow("AppWindow", 
        "App Window",
        WS_OVERLAPPEDWINDOW,
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        win32_main_instance,  // Instance handle
        NULL        // Additional application data
        );

    HDC hdc = GetDC(mainWindow);

    // Default refresh rate will be 60
    int monitorRefreshRate = 60;
    {
        monitorRefreshRate = GetDeviceCaps(hdc, VREFRESH);
        // ReleaseDC(mainWindow, dc);
    }


    // actually make the window visible
    ShowWindow(mainWindow, SW_SHOW);
    HGLRC glcontent = Win32InitOpenGL(hdc);
    wglSwapIntervalEXT(1);  // v-sync on...

    // message pump
    MSG msg = {};
    B32 running = true;
    while(running)
    {
        if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
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

        RECT rc;
        GetClientRect(mainWindow, &rc);
        
        glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);

        glClearColor(0.129f, 0.586f, 0.949f, 1.0f); // rgb(33,150,243) sky blue?
        glClear(GL_COLOR_BUFFER_BIT);

        SwapBuffers(hdc);   // swap main gl buffer to screen
    }

    DestroyWindow(mainWindow);

    return(0);
}
