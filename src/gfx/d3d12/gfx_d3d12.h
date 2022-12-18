#ifndef GFX_D3D12_H
#define GFX_D3D12_H

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <dxgidebug.h>

#include "d3d12_memory_allocator.h"

function void init_api_d3d12(GfxApi* api);

// D3D12 interface structures

typedef S32 DxDescriptorID;

struct D3d12GpuContext;
struct D3d12Fence;
struct D3d12Semaphore;
struct D3d12Queue;
struct D3d12SwapChain;
struct D3d12CommandPool;
struct D3d12Command;
struct D3d12Texture;
struct D3d12Buffer;
struct D3d12RenderTarget;
struct D3d12Sampler;
struct D3d12Shader;
struct D3d12RootSignature;
struct D3d12Pipeline;
struct D3d12PipelineCache;
struct D3d12DescriptorSet;
struct D3d12CommandSignature;
struct D3d12QueryPool;

// Raytracing
struct D3d12Raytracing;
struct GfxAccelerationStructure;    // TODO(james): Is this only valid for DX12?

struct D3d12DescriptorHeap
{
    ID3D12DescriptorHeap*           heap;
    OSMutex                         mutex;
    ID3D12Device*                   device;
    D3D12_CPU_DESCRIPTOR_HANDLE     start_cpu_handle;
    D3D12_GPU_DESCRIPTOR_HANDLE     start_gpu_handle;
    // bitmask to track free regions
    U32*                            flags;
    D3D12_DESCRIPTOR_HEAP_TYPE      type;
    U32                             num_descriptors;
    U32                             descriptor_size;
    U32                             used_descriptors;
};

// TODO(james): Decide if the enums from the shader reflection interface are required
#define TEXTURE_DIM_COUNT 9

struct D3d12NullDescriptors
{
	// Default NULL Descriptors for binding at empty descriptor slots to make sure all descriptors are bound at submit
	DxDescriptorID mNullTextureSRV[TEXTURE_DIM_COUNT];
	DxDescriptorID mNullTextureUAV[TEXTURE_DIM_COUNT];
	DxDescriptorID mNullBufferSRV;
	DxDescriptorID mNullBufferUAV;
	DxDescriptorID mNullBufferCBV;
	DxDescriptorID mNullSampler;
};

struct D3d12GpuContext
{
    D3d12DescriptorHeap**           cpu_descriptor_heaps;
    D3d12DescriptorHeap**           cbv_srv_uav_heaps;
    D3d12DescriptorHeap**           sampler_heaps;
    D3D12MA::Allocator*             resource_allocator;

    IDXGIFactory6*                  dxgi_factory;
    IDXGIAdapter4*                  active_gpu;
    ID3D12Device*                   device;

#if 0
    // TODO(james)
    ID3D12DeviceRemovedExtendedDataSettings* dred_settings;
#else
    U64                             pad_a;
#endif
    ID3D12Debug*                    debug;
    ID3D12InfoQueue*                debug_validation;

#if GFX_ENABLE_NSIGHT_AFTERMATH
# error TODO(james): Actually implement this...
    GfxAftermathTracker             aftermath_tracker;
    B32                             aftermath_support;
    B32                             diag_config_support;
    B32                             diag_check_points_support;
#endif
    D3d12NullDescriptors*           null_descriptors;
    char*                           name;
    GfxGpuSettings*                 active_gpu_settings;
    GfxGpuCapBits*                  cap_bits;
    U32                             linked_node_count;
    U32                             unlinked_renderer_index;
    U32                             gpu_mode;
    U32                             shader_target;
    B32                             enable_gpu_based_validation;
    char*                           api_name;
};

struct D3d12Fence
{
    ID3D12Fence*    fence;
    HANDLE          wait_idle_fence_event;
    U64             fence_value;
};

// DirectX12 does not have a concept of semaphores
// All synchronization is done using fences
// Simlate semaphore signal and wait using DirectX12 fences

// Semaphores used in DirectX12 only in queueSubmit
// queueSubmit -> How the semaphores work in DirectX12

// pp_wait_semaphores -> queue->Wait is manually called on each fence in this
// array before calling ExecuteCommandLists to make the fence work like a wait semaphore

// pp_signal_semaphores -> Manually call queue->Signal on each fence in this array after
// calling ExecuteCommandLists and increment the underlying fence value

// queuePresent does not use the wait semaphore since the swapchain Present function
// already does the synchronization in this case
struct D3d12Semaphore
{
    ID3D12Fence*    fence;
    HANDLE          wait_idle_fence_event;
    U64             fence_value;
};

struct D3d12Queue
{
    ID3D12CommandQueue*     queue;
    D3d12Fence*             fence;
    U32                     type;
    U32                     node_index;
};

struct D3d12SwapChain
{
    D3d12RenderTarget**     render_targets;
    /// Use IDXGISwapChain3 for now since IDXGISwapChain4
    /// isn't supported by older devices.
    IDXGISwapChain3*        swap_chain;
    /// Sync interval to specify how interval for vsync
    U32                     sync_interval : 3;
    U32                     flags : 10;
    U32                     image_count : 3;
    B32                     enable_vsync : 1;
};

struct D3d12CommandPool
{
    ID3D12CommandAllocator* cmd_alloc;
    D3d12Queue*             queue;
};

struct D3d12Command
{
    ID3D12GraphicsCommandList*  cmd_list;

    // cache in beginCmd
    D3d12DescriptorHeap*        bound_heaps[2];
    D3D12_GPU_DESCRIPTOR_HANDLE bound_heap_start_handles[2];

    // Command Buffer State
    const ID3D12RootSignature*  bound_root_signature;
    D3d12DescriptorSet*         bound_descriptor_sets[kDescriptorUpdateFreq_COUNT];
    U16                         bound_descriptor_set_indices[kDescriptorUpdateFreq_COUNT];
    U32                         node_index : 4;
    U32                         type : 3;
    D3d12CommandPool*           cmd_pool;
    D3d12GpuContext*            renderer;
    D3d12Queue*                 queue;
};

struct D3d12VirtualTexture
{

};

struct D3d12Texture
{
    DxDescriptorID              dx_descriptors;
    ID3D12Resource*             resource;
    D3D12MA::Allocation*        allocation;
    U32                         handle_count : 24;
    U32                         uav_start_index;
    D3d12VirtualTexture*        svt;
    U32                         width : 16;
    U32                         height : 16;
    U32                         depth : 16;
    U32                         mip_levels : 5;
    U32                         array_size_minus_one : 11;
    U32                         format : 8;
    U32                         aspect_mask : 4;
    U32                         node_index : 4;
    U32                         sample_count : 5;
    U32                         uav : 1;
    U32                         owns_image : 1;
};

struct D3d12Buffer
{
    void*                       cpu_mapped_address;
    D3D12_GPU_VIRTUAL_ADDRESS   gpu_address;
    DxDescriptorID              dx_descriptors;
    U8                          srv_descriptor_offset;
    U8                          uav_descriptor_offset;
    ID3D12Resource*             resource;
    D3D12MA::Allocation*        allocation;
    U64                         size : 32;
    U64                         descriptors : 20;
    U64                         memory_usage : 3;
    U64                         node_index : 4;
};

struct D3d12RenderTarget
{
    D3d12Texture*               texture;
    DxDescriptorID              dx_descriptors;
    D3d12RenderTarget*          resolve_attachment;
    GfxClearValue               clear_value;
    U32                         array_size : 16;
    U32                         depth : 16;
    U32                         width : 16;
    U32                         height : 16;
    U32                         descriptors : 20;
    U32                         mip_levels : 10;
    U32                         sample_quality : 5;
    TinyImageFormat             format;
    GfxSampleCount              sample_count;
    B32                         enable_vr_multiview;
    B32                         enable_vr_foveated_rendering;
};

struct D3d12Sampler
{
    D3D12_SAMPLER_DESC          desc;
    DxDescriptorID              dx_descriptor;
};

struct D3d12Shader
{
    GfxShaderStage              stages : 31;
    B32                         is_multiview_vr : 1;
    U32                         num_threads_per_group[3];
    IDxcBlobEncoding**          shader_blobs;
    LPCWSTR*                    entry_names;
    // TODO(james): reflection?
    // PipelineReflection*      reflection;
};

struct D3d12DescriptorInfo
{
    const char*                 name;
    U32                         type;
    U32                         dim : 4;
    U32                         root_descriptor : 1;
    U32                         static_sampler : 1;
    U32                         update_freq : 3;
    U32                         size;
    U32                         handle_index;
};

struct D3d12DescriptorIndexMap
{
    char*                       key;
    U32                         value;
};

struct D3d12RootSignature
{
    U32                         descriptor_count;
    GfxPipelineType             pipeline_type;
    D3d12DescriptorInfo*        descriptors;
    D3d12DescriptorIndexMap*    descriptor_name_to_index_map;
    ID3D12RootSignature*        dx_root_signature;
    U8                          view_descriptor_table_root_indices[kDescriptorUpdateFreq_COUNT];
    U8                          sampler_descriptor_table_root_indices[kDescriptorUpdateFreq_COUNT];
    U32                         cumulative_view_descriptor_counts[kDescriptorUpdateFreq_COUNT];
    U32                         cumulative_sampler_descriptor_counts[kDescriptorUpdateFreq_COUNT];
    U16                         view_descriptor_counts[kDescriptorUpdateFreq_COUNT];
    U16                         sampler_descriptor_counts[kDescriptorUpdateFreq_COUNT];
};

struct D3d12Pipeline
{
    ID3D12PipelineState*        pipeline_state;
    ID3D12StateObject*          dxr_pipeline;   // raytracing only
    ID3D12RootSignature*        root_signature;
    GfxPipelineType             type;
    D3D_PRIMITIVE_TOPOLOGY      dx_primitive_topology;
};

struct D3d12PipelineCache
{
    ID3D12PipelineLibrary*      library;
    void*                       data;
};

struct D3d12DescriptorSet
{
    DxDescriptorID              cbv_srv_uav_handle;
    DxDescriptorID              sampler_handle;
    U32                         cbv_srv_uav_stride;
    U32                         sampler_stride;
    D3d12RootSignature*         root_signature;
    U32                         max_sets : 16;
    U32                         update_freq : 3;
    U32                         node_index : 4;
    U32                         cbv_srv_uav_root_index : 4;
    U32                         sampler_root_index : 4;
    U32                         pipline_type : 3;
};

struct D3d12CommandSignature
{
    ID3D12CommandSignature*     dx_handle;
    GfxIndirectArgumentType     draw_type;
    U32                         stride;
};

struct D3d12QueryPool
{
    ID3D12QueryHeap*            dx_query_heap;
    D3D12_QUERY_TYPE            type;
    U32                         count;
};

struct D3d12Raytracing
{
    D3d12GpuContext*            renderer;
    ID3D12Device5*              dxr_device;
    U64                         descriptors_allocated;
};

// API FUNCTIONS

function GfxGpuContext d3d12_initGpu(GfxGpuContextDesc* desc);

function GfxFence d3d12_addFence(GfxGpuContext gc);
function void d3d12_removeFence(GfxGpuContext gc, GfxFence fence);
function GfxSemaphore d3d12_addSemaphore(GfxGpuContext gc);
function void d3d12_removeSemaphore(GfxGpuContext gc, GfxSemaphore semaphore);
function GfxQueue d3d12_addQueue(GfxGpuContext gc, GfxQueueDesc* desc);
function void d3d12_removeQueue(GfxGpuContext gc, GfxQueue queue);
function GfxSwapChain d3d12_addSwapChain(GfxGpuContext gc, GfxSwapChainDesc* desc);
function void d3d12_removeSwapChain(GfxGpuContext gc, GfxSwapChain swapchain);

    // command pool operations
function GfxCommandPool d3d12_addCommandPool( GfxGpuContext gc, GfxCommandPoolDesc* desc);
function void d3d12_removeCommandPool( GfxGpuContext gc, GfxCommandPool commandpool);
function GfxCommand d3d12_addCommand( GfxGpuContext gc, GfxCommandPoolDesc* desc);
function void d3d12_removeCommand( GfxGpuContext gc, GfxCommand cmd);
function void d3d12_addCommands( GfxGpuContext gc, GfxCommandDesc* desc, U32 count, GfxCommand* out_commands);
function void d3d12_removeCommands( GfxGpuContext gc, U32 count, GfxCommand* commands);

    //
    //  All buffer, texture loading handled by the resource system -> GfxResourceLoader
    //
function GfxBuffer d3d12_addBuffer(GfxGpuContext gc, GfxBufferDesc* desc);
function void d3d12_removeBuffer(GfxGpuContext gc, GfxBuffer buffer);
function void d3d12_mapBuffer(GfxGpuContext gc, GfxBuffer buffer, GfxReadRange* range);
function void d3d12_unmapBuffer(GfxGpuContext gc, GfxBuffer buffer);
function void d3d12_cmdUpdateBuffer(GfxCommand cmd, GfxBuffer buffer, U64 dstOffset, GfxBuffer srcBuffer, U64 srcOffset, U64 size);
function void d3d12_cmdUpdateSubresource(GfxCommand cmd, GfxTexture texture, GfxBuffer srcBuffer, GfxSubResourceDataDesc* desc);
function void d3d12_cmdCopySubresource(GfxCommand cmd, GfxBuffer dstBuffer, GfxTexture texture, GfxSubResourceDataDesc* desc);
function GfxTexture d3d12_addTexture(GfxGpuContext gc, GfxTextureDesc* desc);
function void d3d12_removeTexture(GfxGpuContext gc, GfxTexture texture);
function void d3d12_addVirtualTexture(GfxCommand cmd, GfxTextureDesc* desc, GfxTexture textures, void* image_data);
    //

function GfxRenderTarget d3d12_addRenderTarget(GfxGpuContext gc, GfxRenderTargetDesc* desc);
function void d3d12_removeRenderTarget(GfxGpuContext gc, GfxRenderTarget rendertarget);
function GfxSampler d3d12_addSampler(GfxGpuContext gc, GfxSamplerDesc* desc);
function void d3d12_removeSampler(GfxGpuContext gc, GfxSampler sampler);

    // shader operations
function GfxShader d3d12_addShaderBinary(GfxGpuContext gc, GfxShaderDesc* desc);
function void d3d12_removeShader(GfxGpuContext gc, GfxShader shader);

function GfxRootSignature d3d12_addRootSignature(GfxGpuContext gc, GfxRootSignatureDesc* desc);
function void d3d12_removeRootSignature(GfxGpuContext gc, GfxRootSignature rootsignature);

    // pipeline operations
function GfxPipeline d3d12_addPipeline(GfxGpuContext gc, GfxPipelineDesc* desc);
function void d3d12_removePipeline(GfxGpuContext gc, GfxPipeline pipeline);
function GfxPipelineCache d3d12_addPipelineCache(GfxGpuContext gc, GfxPipelineCacheDesc* desc);
function void d3d12_getPipelineCacheData(GfxGpuContext gc, GfxPipelineCache pipelinecache, U64* out_size, void* out_data);
function void d3d12_removePipelineCache(GfxGpuContext gc, GfxPipelineCache pipelinecache);

    // descriptor set operations
function GfxDescriptorSet d3d12_addDescriptorSet(GfxGpuContext gc, GfxDescriptorSetDesc* desc);
function void d3d12_removeDescriptorSet(GfxGpuContext gc, GfxDescriptorSet descriptorset);
function void d3d12_updateDescriptorSet(GfxGpuContext gc, U32 index, GfxDescriptorSet descriptorset, U32 count, GfxDescriptorData* params);

    // command buffer operations
function void d3d12_resetCommandPool(GfxGpuContext gc, GfxCommandPool commandpool);
function void d3d12_beginCommand(GfxCommand cmd);
function void d3d12_endCommand(GfxCommand cmd);
function void d3d12_cmdBindRenderTargets(GfxCommand cmd, U32 render_target_count, GfxRenderTarget* renderTargets, GfxRenderTarget depth_stencil, GfxLoadActionsDesc* loadActions, U32* color_array_slices, U32* color_mip_slices, U32 depth_array_slice, U32 depth_mip_slice);
function void d3d12_cmdSetShadingRate(GfxCommand cmd, GfxShadingRate shading_rate, GfxTexture texture, GfxShadingRateCombiner post_rasterizer_rate, GfxShadingRateCombiner final_rate);
function void d3d12_cmdSetViewport(GfxCommand cmd, F32 x, F32 y, F32 width, F32 height, F32 min_depth, F32 max_depth);
function void d3d12_cmdSetScissor(GfxCommand cmd, U32 x, U32 y, U32 width, U32 height);
function void d3d12_cmdSetStencilReferenceValue(GfxCommand cmd, U32 val);
function void d3d12_cmdBindPipeline(GfxCommand cmd, GfxPipeline pipeline);
function void d3d12_cmdBindDescriptorSet(GfxCommand cmd, U32 index, GfxDescriptorSet descriptorset);
function void d3d12_cmdBindPushConstants(GfxCommand cmd, GfxRootSignature rootsignature, U32 index, void* constant_data);
function void d3d12_cmdBindDescriptorSetWithRootCbvs(GfxCommand cmd, U32 index, GfxDescriptorSet descriptorset, U32 count, GfxDescriptorData* params);
function void d3d12_cmdBindIndexBuffer(GfxCommand cmd, GfxBuffer buffer, U32 index_type, U64 offset);
function void d3d12_cmdBindVertexBuffer(GfxCommand cmd, U32 count, GfxBuffer* buffers, U32* strides, U64* offsets);
function void d3d12_cmdDraw(GfxCommand cmd, U32 vertex_count, U32 first_vertex);
function void d3d12_cmdDrawInstanced(GfxCommand cmd, U32 vertex_count, U32 first_vertex, U32 instance_count, U32 first_instance);
function void d3d12_cmdDrawIndexed(GfxCommand cmd, U32 index_count, U32 first_index, U32 first_vertex);
function void d3d12_cmdDrawIndexedInstanced(GfxCommand cmd, U32 index_count, U32 first_index, U32 instance_count, U32 first_vertex, U32 first_instance);
function void d3d12_cmdDispatch(GfxCommand cmd, U32 group_count_x, U32 group_count_y, U32 group_count_z);

    // transition commands
function void d3d12_cmdResourceBarrier(GfxCommand cmd, U32 buffer_barrier_count, GfxBufferBarrier* buffer_barriers, U32 texture_barrier_count, GfxTextureBarrier* texture_barriers, U32 rendertarget_barrier_count, GfxRenderTargetBarrier* rendertarget_barriers);

    // virtual textures
function void d3d12_cmdUpdateVirtualTexture(GfxCommand cmd, GfxTexture texture, U32 current_image);

    // queue/fence/swapchain operations
function void d3d12_acquireNextImage(GfxGpuContext gc, GfxSwapChain swapchain, GfxSemaphore signal_semaphore, GfxFence fence, U32* out_image_index);
function void d3d12_queueSubmit(GfxQueue queue, GfxQueueSubmitDesc* desc);
function void d3d12_queuePresent(GfxQueue queue, GfxQueuePresentDesc* desc);
function void d3d12_waitQueueIdle(GfxQueue queue);
function GfxFenceStatus d3d12_getFenceStatus(GfxGpuContext gc, GfxFence fence);
function void d3d12_waitForFences(GfxGpuContext gc, U32 count, GfxFence* fences);
function void d3d12_toggleVSync(GfxGpuContext gc, GfxSwapChain swapchain);

    //Returns the recommended format for the swapchain.
    //If true is passed for the hintHDR parameter, it will return an HDR format IF the platform supports it
    //If false is passed or the platform does not support HDR a non HDR format is returned.
    //If true is passed for the hintSrgb parameter, it will return format that is will do gamma correction automatically
    //If false is passed for the hintSrgb parameter the gamma correction should be done as a postprocess step before submitting image to swapchain
function TinyImageFormat d3d12_getRecommendedSwapchainFormat(bool hintHDR, bool hintSRGB);

    // indirect draw operations
function GfxCommandSignature d3d12_addIndirectCommandSignature(GfxGpuContext gc, GfxCommandSignatureDesc* desc);
function void d3d12_removeIndirectCommandSignature(GfxGpuContext gc, GfxCommandSignature cmdsignature);
function void d3d12_cmdExecuteIndirect(GfxCommand cmd, GfxCommandSignature cmdsignature, U32 max_cmd_count, GfxBuffer indirect_buffer, U64 buffer_offset, GfxBuffer counter_buffer, U64 counter_buffer_offset);

    //
    // GPU Query Interface
    //
function F64 d3d12_getTimestampFrequency(GfxQueue queue);
function GfxQueryPool d3d12_addQueryPool(GfxGpuContext gc, GfxQueryPoolDesc* desc);
function void d3d12_removeQueryPool(GfxGpuContext gc, GfxQueryPool querypool);
function void d3d12_cmdResetQueryPool(GfxCommand cmd, GfxQueryPool querypool, U32 start_query, U32 query_count);
function void d3d12_cmdBeginQuery(GfxCommand cmd, GfxQueryPool querypool, GfxQueryDesc* desc);
function void d3d12_cmdEndQuery(GfxCommand cmd, GfxQueryPool querypool, GfxQueryDesc* desc);
function void d3d12_cmdResolveQuery(GfxCommand cmd, GfxQueryPool querypool, GfxBuffer readback_buffer, U32 start_query, U32 query_count);
    //
    // Stats Info Interface
    //
function void d3d12_calculateMemoryStats(MemArena* arena, GfxGpuContext gc, String8List* out_stats);
function void d3d12_calculateMemoryUse(GfxGpuContext gc, U64* out_usedbytes, U64* out_totalallocatedbytes);
    //
    // Debug Marker Interface
    //
function void d3d12_cmdAddDebugMarker(GfxCommand cmd, F32 r, F32 g, F32 b, String8 name);
function void d3d12_cmdBeginDebugMarker(GfxCommand cmd, F32 r, F32 g, F32 b, String8 name);
function void d3d12_cmdEndDebugMarker(GfxCommand cmd);
function U32 d3d12_cmdWriteMarker(GfxCommand cmd, GfxMarkerType marker_type, U32 marker_value, GfxBuffer buffer, U64 offset, B32 use_auto_flags);
    //
    // Resource Debug Naming Interface
    //
function void d3d12_setBufferName(GfxGpuContext gc, GfxBuffer buffer, String8 name);
function void d3d12_setTextureName(GfxGpuContext gc, GfxTexture texture, String8 name);
function void d3d12_setRenderTargetName(GfxGpuContext gc, GfxRenderTarget rendertarget, String8 name);
function void d3d12_setPipelineName(GfxGpuContext gc, GfxPipeline pipeline, String8 name);
    //
function U32 d3d12_getDescriptorIndexFromName(GfxRootSignature rootsignature, String8 name);


#endif // GFX_D3D12_H
