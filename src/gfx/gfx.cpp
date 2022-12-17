#include "gfx.h"

#if DX11

#endif

#if DX12
#include "d3d12/gfx_d3d12.cpp"
#endif

#if VULKAN

#endif

#if METAL

#endif

#if GLES

#endif

function GfxApi gfx_init_api(GfxApiType type)
{
    GfxApi api = {};

    if(type == kGfxApi_PlatformDefault)
    {
#if OS_WINDOWS
        type = kGfxApi_D3D12;
#elif OS_MACOS
        type = kGfxApi_Metal;
#elif OS_LINUX
        type = kGfxApi_Vulkan;
#else
# error OS Platform does not have a default graphics api specified
#endif
    }

    switch(type)
    {
#if DX11
        case kGfxApi_D3D11:
            init_api_d3d11(&api);
            break;
#endif
#if DX12
        case kGfxApi_D3D12:
            init_api_d3d12(&api);
            break;
#endif
#if METAL
        case kGfxApi_Metal:
            init_api_metal(&api);
            break;
#endif
#if VULKAN
        case kGfxApi_Vulkan:
            init_api_vulkan(&api);
            break;
#endif
#if GLES
        case kGfxApi_GLES:
            init_api_gles(&api);
            break;
#endif
        default:
            Assert(false);
            break;
    }


    return api;
}
