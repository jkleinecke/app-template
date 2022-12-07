
#include "os_helpers.cpp"

#if OS_WINDOWS
# include "win32/win32_essential.h"
# include "win32/win32_window.h"
# include "win32/win32_essential.cpp"
# include "win32/win32_window.cpp"
# include "win32/win32_entrypoint.cpp"
#elif OS_MACOS
# include "macos/macos_essential.h"
# include "macos/macos_window.h"
# include "macos/macos_essential.mm"
# include "macos/macos_window.mm"
# include "macos/macos_entrypoint.mm"
#else
# error no backend for os_inc.cpp on this OS
#endif

