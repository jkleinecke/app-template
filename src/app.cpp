#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.cpp"
#include "os/os_inc.cpp"


int app_main(void)
{
    printf("Hello World!\n");

    String8List cmd_args = os_command_line_args();
    for(String8Node* node = cmd_args.first; node; node = node->next)
    {
        printf("Arg: %s\n", node->string.str);
    }

    return(0);
}
