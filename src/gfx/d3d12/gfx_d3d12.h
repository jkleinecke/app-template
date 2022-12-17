#ifndef GFX_D3D12_H
#define GFX_D3D12_H

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

function void init_api_d3d12(GfxApi* api);

// D3D12 interface structures

// typedef void* GfxGpuContext;
// typedef void* GfxFence;
// typedef void* GfxSemaphore;
// typedef void* GfxQueue;
// typedef void* GfxSwapChain;
// typedef void* GfxCommandPool;
// typedef void* GfxCommand;
// typedef void* GfxTexture;
// typedef void* GfxBuffer;
// typedef void* GfxRenderTarget;
// typedef void* GfxSampler;
// typedef void* GfxShader;
// typedef void* GfxRootSignature;
// typedef void* GfxPipeline;
// typedef void* GfxPipelineCache;
// typedef void* GfxDescriptorSet;
// typedef void* GfxCommandSignature;
// typedef void* GfxQueryPool;

// // Raytracing
// typedef void* GfxRaytracing;
// struct GfxAccelerationStructure;    // TODO(james): Is this only valid for DX12?

#endif // GFX_D3D12_H
