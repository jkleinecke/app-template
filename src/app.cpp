#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.cpp"
#include "os/os_inc.cpp"

#include "gfx/gfx.cpp"

int app_main(void)
{
    OSWindow window = os_window_create(str8_lit("Test App"), 512, 512);

    B32 running = true;
    while(running)
    {
        OSEventType event_type = os_poll_window_events();

        switch(event_type)
        {
        case kOSEventType_NeedsRender:
        {
            os_window_render_begin(window);

            // TODO: Render Application Here

            os_window_render_end(window);
        }break;
        case kOSEventType_Quit:
            running = false;
            break;
        case kOSEventType_Idle:
            // do nothing, maybe sleep?
            break;
        default:
            break;
        }
    }


    os_window_destroy(window);

    return(0);
}
