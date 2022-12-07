#ifndef MACOS_ESSENTIAL_H
#define MACOS_ESSENTIAL_H

#pragma push_macro("local")
#pragma push_macro("function")
#pragma push_macro("global")
#undef local
#undef function
#undef global
#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#pragma pop_macro("local")
#pragma pop_macro("function")
#pragma pop_macro("global")

@interface MacosAppDelegate : NSObject<NSApplicationDelegate, NSWindowDelegate>
@end

NSString* nsstring_from_str8(MemArena* arena, String8 str);
String8 str8_from_nsstring(MemArena* arena, NSString* nsstr);

#endif
