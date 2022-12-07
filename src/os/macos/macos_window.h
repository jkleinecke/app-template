#ifndef MACOS_WINDOW_H
#define MACOS_WINDOW_H

struct MacosWindow
{
    MemArena* arena;
    NSWindow* window;
    NSView* view;
    NSOpenGLContext* gl_context;
};

void MacosCreateSimpleMainMenu(NSString* AppName);
void MacosProcessMinimalPendingMessages();

#endif
