
#include "macos_window_opengl.h"

// #include "imgui/backends/imgui_impl_osx.h"
// #include "imgui/backends/imgui_impl_opengl3.h"


///////////////////////////////////////////////////////////////////////
// Application Delegate

@implementation MacosAppDelegate

// app delegate methods
- (void)applicationDidFinishLaunching:(id)sender
{
	#pragma unused(sender)
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	#pragma unused(sender)

	return YES;
}


- (void)applicationWillTerminate:(NSApplication*)sender
{
	#pragma unused(sender)
}


// window delegate methods
- (NSSize)windowWillResize:(NSWindow*)window
                    toSize:(NSSize)frameSize
{
	// Maintain the proper aspect ratio...

	// NSRect WindowRect = [window frame];
	// NSRect ContentRect = [window contentRectForFrameRect:WindowRect];

	// F32 WidthAdd = (WindowRect.size.width - ContentRect.size.width);
	// F32 HeightAdd = (WindowRect.size.height - ContentRect.size.height);

	//F32 NewCy = (GlobalRenderHeight * (frameSize.width - WidthAdd)) / GlobalRenderWidth;
	// F32 NewCy = (GlobalAspectRatio.Height * (frameSize.width - WidthAdd)) / GlobalAspectRatio.Width;

	// frameSize.height = NewCy + HeightAdd;

	return frameSize;
}

- (void)windowWillClose:(id)sender
{
	macos_didquit = true;
}

@end

// Application Delegate
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// OpenGL View

@interface AppGLView : NSOpenGLView
{
}
@end

@implementation AppGLView

- (id)init
{
	self = [super init];

	return self;
}


- (void)prepareOpenGL
{
	[super prepareOpenGL];
	[[self openGLContext] makeCurrentContext];
}

NSOpenGLContext* InternalGLContext;

- (void)reshape
{
	[super reshape];

	NSRect bounds = [self bounds];
	[InternalGLContext makeCurrentContext];
	[InternalGLContext update];
	glViewport(0, 0, bounds.size.width, bounds.size.height);
}

@end

// OpenGL View
/////////////////////////////////////////////////////////////////////

void MacosCreateSimpleMainMenu(NSString* AppName)
{
    NSMenu* menubar = [NSMenu new];

	NSMenuItem* appMenuItem = [NSMenuItem new];
	[menubar addItem:appMenuItem];

	[NSApp setMainMenu:menubar];

	NSMenu* appMenu = [NSMenu new];

    // [appMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Enter Full Screen"
	// 				  						    action:@selector(toggleFullScreen:)
	// 									 keyEquivalent:@"f"]];

    [appMenu addItem:[[NSMenuItem alloc] initWithTitle:[@"Quit " stringByAppendingString:AppName]
											    action:@selector(terminate:)
										 keyEquivalent:@"q"]];
    [appMenuItem setSubmenu:appMenu];
}

function OSWindow os_window_create(String8 title, U32 width, U32 height)
{
	MemArena* arena = mem_alloc_arena();

	// // TODO: Only call create context for the main window...
	// ImGui::CreateContext();
    // ImGui::StyleColorsDark();

	MacosWindow* windowcontext = push_struct(arena, MacosWindow);
	windowcontext->arena = arena;
	
    NSRect ScreenRect = [[NSScreen mainScreen] frame];

	NSRect InitialFrame = NSMakeRect((ScreenRect.size.width - width) * 0.5,
									(ScreenRect.size.height - height) * 0.5,
									width,
									height);

	NSWindow* window = [[NSWindow alloc] initWithContentRect:InitialFrame
									styleMask:NSWindowStyleMaskTitled
												| NSWindowStyleMaskClosable
												| NSWindowStyleMaskMiniaturizable
												| NSWindowStyleMaskResizable
									backing:NSBackingStoreBuffered
									defer:NO];

	[window setBackgroundColor: NSColor.redColor];
	[window setDelegate: macos_appdelegate];

	NSView* CV = [window contentView];
	[CV setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
	[CV setAutoresizesSubviews:YES];

	// All hardware NSOpenGLPixelFormats support an sRGB framebuffer.
    NSOpenGLPixelFormatAttribute openGLAttributes[] =
    {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        0
    };
	NSOpenGLPixelFormat* PixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:openGLAttributes];
    NSOpenGLContext* GLContext = [[NSOpenGLContext alloc] initWithFormat:PixelFormat shareContext:NULL];

	AppGLView* GLView = [[AppGLView alloc] init];
	[GLView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
	[GLView setPixelFormat:PixelFormat];
	[GLView setOpenGLContext:GLContext];
	[GLView setFrame:[CV bounds]];

	[CV addSubview:GLView];

    [PixelFormat release];

	MemArenaTemp scratch = mem_get_scratch(&arena, 1);

	NSString* windowTitle = nsstring_from_str8(scratch.arena, title);

	[window setMinSize:NSMakeSize(160, 90)];
	[window setTitle:windowTitle];
	[window makeKeyAndOrderFront:nil];

	// ImGui_ImplOSX_Init(GLView);

	///////////////////////////////////////////////////////////////////
	// OpenGL setup with Cocoa
    GLint swapInt = 1;	// 1 enables V-Sync
	[GLContext setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

	[GLContext setView:[window contentView]];
	[GLContext makeCurrentContext];

	// ImGui_ImplOpenGL3_Init();

	windowcontext->window = window;
	windowcontext->view = GLView;
	windowcontext->gl_context = GLContext;

	mem_release_scratch(scratch);

    return windowcontext;
}

function void os_window_destroy(OSWindow window)
{
	MacosWindow* windowcontext = (MacosWindow*) window;

	// ImGui_ImplOpenGL3_Shutdown();
	// ImGui_ImplOSX_Shutdown();

	// TODO: destroy the window
	NSWindowController* controller = windowcontext->window.windowController;
	[controller close];

	mem_arena_release(windowcontext->arena);
}

function void os_window_needs_refresh(OSWindow window)
{
	MacosWindow* windowcontext = (MacosWindow*) window;
	
	windowcontext->view.needsDisplay = YES;
}

function void os_window_render_begin(OSWindow window)
{
	MacosWindow* windowcontext = (MacosWindow*) window;
	NSOpenGLContext* GLContext = windowcontext->gl_context;

	[GLContext makeCurrentContext];

	// ImGui_ImplOpenGL3_NewFrame();
    // ImGui_ImplOSX_NewFrame(windowcontext->view);
    // ImGui::NewFrame();
}

function void os_window_render_end(OSWindow window)
{
	MacosWindow* windowcontext = (MacosWindow*) window;
	NSOpenGLContext* GLContext = windowcontext->gl_context;

	// ImGui::Render();
	// ImDrawData* draw_data = ImGui::GetDrawData();

    // GLsizei width  = (GLsizei)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    // GLsizei height = (GLsizei)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    // glViewport(0, 0, width, height);

    glClearColor(0.129f, 0.586f, 0.949f, 1.0f); // rgb(33,150,243) sky blue?
    glClear(GL_COLOR_BUFFER_BIT);

    // ImGui_ImplOpenGL3_RenderDrawData(draw_data);

	// flush buffer forces the v-sync to occur
	[GLContext flushBuffer];
}

function OSEventType os_poll_window_events(void)
{
	NSEvent* Event;

	OSEventType result_eventType = kOSEventType_Idle;
	U64 sleeptime = 5000;

	int loopcounter = 0;
	do
	{
		loopcounter += 1;
		NSEventMask eventMask = NSEventMaskAny;
		// eventMask |= NSEventMaskMouseMoved | NSEventMaskScrollWheel;
		// eventMask |= NSEventMaskLeftMouseDown | NSEventMaskLeftMouseUp | NSEventMaskLeftMouseDragged;
		// eventMask |= NSEventMaskRightMouseDown | NSEventMaskRightMouseUp | NSEventMaskRightMouseDragged;
		// eventMask |= NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp | NSEventMaskOtherMouseDragged;
		// eventMask |= NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged;

		Event = [NSApp nextEventMatchingMask:eventMask
									untilDate:nil
										inMode:NSDefaultRunLoopMode
									dequeue:YES];

		if(Event != nil)
		{
			sleeptime = 0;
		
			switch ([Event type])
			{
				// case NSEventTypeKeyDown:
				// case NSEventTypeKeyUp:
				// {
				// 	U32 KeyCode = [Event keyCode];
				// 	if (KeyCode == kVK_ANSI_Q)
				// 	{
				// 		macos_didquit = true;
				// 	}
				// } break;

				default:
					[NSApp sendEvent:Event];
			}
		}
	} while (Event != nil);

	if(sleeptime && loopcounter <= 1)
	{
		usleep(sleeptime);
	}
	else
	{
		result_eventType = kOSEventType_NeedsRender;
	}

    return macos_didquit ? kOSEventType_Quit : result_eventType;
}
