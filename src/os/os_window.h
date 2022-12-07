#ifndef OS_WINDOW_H_
#define OS_WINDOW_H_

typedef void* OSWindow;

function OSWindow os_window_create(String8 title, U32 width, U32 height);
function void os_window_destroy(OSWindow window);

function void os_window_needs_refresh(OSWindow window);

function void os_window_render_begin(OSWindow window);
function void os_window_render_end(OSWindow window);

enum OSEventType
{
    kOSEventType_Quit,
    kOSEventType_NeedsRender,
    kOSEventType_Idle,
};

function OSEventType os_poll_window_events(void); // returns false if app should quit

#endif // OS_WINDOW_H_
