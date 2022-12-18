#include "gfx_d3d12.h"

// TODO(james): Is this the best way to include the library linkages?
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


#define IID_ARGS IID_PPV_ARGS

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN ((D3D12_GPU_VIRTUAL_ADDRESS)-1)
#define D3D12_REQ_CONSTANT_BUFFER_SIZE (D3D12_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * 16u)
#define D3D12_DESCRIPTOR_ID_NONE ((int32_t)-1)

// #define SAFE_FREE(p_var)         \
// 	if ((p_var))                 \
// 	{                            \
// 		tf_free((void*)(p_var)); \
// 		p_var = NULL;            \
// 	}

#if defined(__cplusplus)
#define DECLARE_ZERO(type, var) type var = {};
#else
#define DECLARE_ZERO(type, var) type var = { 0 };
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p_var) \
	if (p_var)              \
	{                       \
		p_var->Release();   \
		p_var = NULL;       \
	}
#endif

#define CALC_SUBRESOURCE_INDEX(MipSlice, ArraySlice, PlaneSlice, MipLevels, ArraySize) \
	((MipSlice) + ((ArraySlice) * (MipLevels)) + ((PlaneSlice) * (MipLevels) * (ArraySize)))

// TODO(james): Log out error
//         LOGF(eERROR, "%s: FAILED with HRESULT: %u", #exp, (uint32_t)hres);
#define CHECK_HRESULT(exp)                                                     \
do                                                                         \
{                                                                          \
    HRESULT hres = (exp);                                                  \
    if (!SUCCEEDED(hres))                                                  \
    {                                                                      \
        Assert(false);                                                     \
    }                                                                      \
} while (0)

// clang-format off
D3D12_BLEND_OP gDx12BlendOpTranslator[kBlendMode_COUNT] =
{
	D3D12_BLEND_OP_ADD,
	D3D12_BLEND_OP_SUBTRACT,
	D3D12_BLEND_OP_REV_SUBTRACT,
	D3D12_BLEND_OP_MIN,
	D3D12_BLEND_OP_MAX,
};

D3D12_BLEND gDx12BlendConstantTranslator[kBlendConstant_COUNT] =
{
	D3D12_BLEND_ZERO,
	D3D12_BLEND_ONE,
	D3D12_BLEND_SRC_COLOR,
	D3D12_BLEND_INV_SRC_COLOR,
	D3D12_BLEND_DEST_COLOR,
	D3D12_BLEND_INV_DEST_COLOR,
	D3D12_BLEND_SRC_ALPHA,
	D3D12_BLEND_INV_SRC_ALPHA,
	D3D12_BLEND_DEST_ALPHA,
	D3D12_BLEND_INV_DEST_ALPHA,
	D3D12_BLEND_SRC_ALPHA_SAT,
	D3D12_BLEND_BLEND_FACTOR,
	D3D12_BLEND_INV_BLEND_FACTOR,
};

D3D12_COMPARISON_FUNC gDx12ComparisonFuncTranslator[kCompare_COUNT] =
{
	D3D12_COMPARISON_FUNC_NEVER,
	D3D12_COMPARISON_FUNC_LESS,
	D3D12_COMPARISON_FUNC_EQUAL,
	D3D12_COMPARISON_FUNC_LESS_EQUAL,
	D3D12_COMPARISON_FUNC_GREATER,
	D3D12_COMPARISON_FUNC_NOT_EQUAL,
	D3D12_COMPARISON_FUNC_GREATER_EQUAL,
	D3D12_COMPARISON_FUNC_ALWAYS,
};

D3D12_STENCIL_OP gDx12StencilOpTranslator[kStencilOp_COUNT] =
{
	D3D12_STENCIL_OP_KEEP,
	D3D12_STENCIL_OP_ZERO,
	D3D12_STENCIL_OP_REPLACE,
	D3D12_STENCIL_OP_INVERT,
	D3D12_STENCIL_OP_INCR,
	D3D12_STENCIL_OP_DECR,
	D3D12_STENCIL_OP_INCR_SAT,
	D3D12_STENCIL_OP_DECR_SAT,
};

D3D12_CULL_MODE gDx12CullModeTranslator[kCull_COUNT] =
{
	D3D12_CULL_MODE_NONE,
	D3D12_CULL_MODE_BACK,
	D3D12_CULL_MODE_FRONT,
};

D3D12_FILL_MODE gDx12FillModeTranslator[kFill_COUNT] =
{
	D3D12_FILL_MODE_SOLID,
	D3D12_FILL_MODE_WIREFRAME,
};

const D3D12_COMMAND_LIST_TYPE gDx12CmdTypeTranslator[kQueueType_COUNT] =
{
	D3D12_COMMAND_LIST_TYPE_DIRECT,
	D3D12_COMMAND_LIST_TYPE_COPY,
	D3D12_COMMAND_LIST_TYPE_COMPUTE
};

const D3D12_COMMAND_QUEUE_PRIORITY gDx12QueuePriorityTranslator[kQueuePriority_COUNT]
{
	D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
	D3D12_COMMAND_QUEUE_PRIORITY_HIGH,
 	D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME,
};

function void init_api_d3d12(GfxApi* api)
{
    
    NotImplemented;
}

U64                         util_dx12_determine_storage_counter_offset(U64 buffer_size);
DXGI_FORMAT                 util_to_dx12_uav_format(DXGI_FORMAT defaultFormat);
DXGI_FORMAT                 util_to_dx12_dsv_format(DXGI_FORMAT defaultFormat);
DXGI_FORMAT                 util_to_dx12_srv_format(DXGI_FORMAT defaultFormat);
DXGI_FORMAT                 util_to_dx12_stencil_format(DXGI_FORMAT defaultFormat);
DXGI_FORMAT                 util_to_dx12_swapchain_format(TinyImageFormat format);
D3D12_SHADER_VISIBILITY     util_to_dx12_shader_visibility(GfxShaderStage stages);
D3D12_DESCRIPTOR_RANGE_TYPE util_to_dx12_descriptor_range(GfxDescriptorType type);
D3D12_RESOURCE_STATES       util_to_dx12_resource_state(GfxResourceState state);
D3D12_FILTER				  util_to_dx12_filter(GfxFilterType minFilter, GfxFilterType magFilter, GfxMipMapMode mipMapMode, bool aniso, bool comparisonFilterEnabled);
D3D12_TEXTURE_ADDRESS_MODE    util_to_dx12_texture_address_mode(GfxAddressMode addressMode);
D3D12_PRIMITIVE_TOPOLOGY_TYPE util_to_dx12_primitive_topology_type(GfxPrimitiveTopology topology);

// Functions points for functions that need to be loaded
PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER           fnD3D12CreateRootSignatureDeserializer = NULL;
PFN_D3D12_SERIALIZE_VERSIONED_ROOT_SIGNATURE           fnD3D12SerializeVersionedRootSignature = NULL;
PFN_D3D12_CREATE_VERSIONED_ROOT_SIGNATURE_DESERIALIZER fnD3D12CreateVersionedRootSignatureDeserializer = NULL;

struct DescriptorHeapProperties
{
	U32                         max_descriptors;
	D3D12_DESCRIPTOR_HEAP_FLAGS flags;
};

DescriptorHeapProperties gCpuDescriptorHeapProperties[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
{
	{ 1024 * 256, D3D12_DESCRIPTOR_HEAP_FLAG_NONE },    // CBV SRV UAV
	{ 2048, D3D12_DESCRIPTOR_HEAP_FLAG_NONE },          // Sampler
	{ 512, D3D12_DESCRIPTOR_HEAP_FLAG_NONE },           // RTV
	{ 512, D3D12_DESCRIPTOR_HEAP_FLAG_NONE },           // DSV
};


/************************************************************************/
// Static Descriptor Heap Implementation
/************************************************************************/
function void add_descriptor_heap(MemArena* arena, ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC* pDesc, D3d12DescriptorHeap** ppDescHeap)
{
	U32 numDescriptors = pDesc->NumDescriptors;
	// hook_modify_descriptor_heap_size(pDesc->Type, &numDescriptors);

	// Keep 32 aligned for easy remove
	numDescriptors = AlignUpPow2(numDescriptors, 32);

	const size_t sizeInBytes = (numDescriptors / 32) * sizeof(U32);

	D3d12DescriptorHeap* pHeap = push_struct(arena, D3d12DescriptorHeap);
	pHeap->flags = (U32*)(pHeap + 1);
	pHeap->device = pDevice;

	pHeap->mutex = os_init_mutex(arena);

	D3D12_DESCRIPTOR_HEAP_DESC desc = *pDesc;
	desc.NumDescriptors = numDescriptors;

	CHECK_HRESULT(pDevice->CreateDescriptorHeap(&desc, IID_ARGS(&pHeap->heap)));

	pHeap->start_cpu_handle = pHeap->heap->GetCPUDescriptorHandleForHeapStart();
	if (desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
	{
		pHeap->start_gpu_handle = pHeap->heap->GetGPUDescriptorHandleForHeapStart();
	}
	pHeap->num_descriptors = desc.NumDescriptors;
	pHeap->type = desc.Type;
	pHeap->descriptor_size = pDevice->GetDescriptorHandleIncrementSize(pHeap->type);

	*ppDescHeap = pHeap;
}

function void reset_descriptor_heap(D3d12DescriptorHeap* pHeap)
{
	memset(pHeap->flags, 0, (pHeap->num_descriptors / 32) * sizeof(U32));
	pHeap->used_descriptors = 0;
}

function void remove_descriptor_heap(D3d12DescriptorHeap* pHeap)
{
	SAFE_RELEASE(pHeap->heap);
	os_destroy_mutex(&pHeap->mutex);
	// SAFE_FREE(pHeap);
}

function void return_descriptor_handles_unlocked(D3d12DescriptorHeap* pHeap, DxDescriptorID handle, U32 count)
{
	if (D3D12_DESCRIPTOR_ID_NONE == handle || !count)
	{
		return;
	}

	for (uint32_t id = handle; id < handle + count; ++id)
	{
		const uint32_t i = id / 32;
		const uint32_t mask = ~(1 << (id % 32));
		pHeap->flags[i] &= mask;
	}

	pHeap->used_descriptors -= count;
}

function void return_descriptor_handles(D3d12DescriptorHeap* pHeap, DxDescriptorID handle, U32 count)
{
    os_aquire_mutex(pHeap->mutex);
	return_descriptor_handles_unlocked(pHeap, handle, count);
    os_release_mutex(pHeap->mutex);
}

function DxDescriptorID consume_descriptor_handles(D3d12DescriptorHeap* pHeap, U32 descriptorCount)
{
	if (!descriptorCount)
	{
		return D3D12_DESCRIPTOR_ID_NONE;
	}

    os_aquire_mutex(pHeap->mutex);

	DxDescriptorID result = D3D12_DESCRIPTOR_ID_NONE;
	DxDescriptorID firstResult = D3D12_DESCRIPTOR_ID_NONE;
	U32 foundCount = 0;

	for (U32 i = 0; i < pHeap->num_descriptors / 32; ++i)
	{
		const U32 flag = pHeap->flags[i];
		if (max_U32 == flag)
		{
			return_descriptor_handles_unlocked(pHeap, firstResult, foundCount);
			foundCount = 0;
			result = D3D12_DESCRIPTOR_ID_NONE;
			firstResult = D3D12_DESCRIPTOR_ID_NONE;
			continue;
		}

		for (int32_t j = 0, mask = 1; j < 32; ++j, mask <<= 1)
		{
			if (!(flag & mask))
			{
				pHeap->flags[i] |= mask;
				result = i * 32 + j;

				Assert(result != D3D12_DESCRIPTOR_ID_NONE && "Out of descriptors");

				if (D3D12_DESCRIPTOR_ID_NONE == firstResult)
				{
					firstResult = result;
				}

				++foundCount;
				++pHeap->used_descriptors;

				if (foundCount == descriptorCount)
				{
                    os_release_mutex(pHeap->mutex);
					return firstResult;
				}
			}
			// Non contiguous. Start scanning again from this point
			else if (foundCount)
			{
				return_descriptor_handles_unlocked(pHeap, firstResult, foundCount);
				foundCount = 0;
				result = D3D12_DESCRIPTOR_ID_NONE;
				firstResult = D3D12_DESCRIPTOR_ID_NONE;
			}
		}
	}

	Assert(result != D3D12_DESCRIPTOR_ID_NONE && "Out of descriptors");
    os_release_mutex(pHeap->mutex);
	return firstResult;
}

inline_function constexpr D3D12_CPU_DESCRIPTOR_HANDLE descriptor_id_to_cpu_handle(D3d12DescriptorHeap* pHeap, DxDescriptorID id)
{
	return { pHeap->start_cpu_handle.ptr + id * pHeap->descriptor_size };
}

inline_function constexpr D3D12_GPU_DESCRIPTOR_HANDLE descriptor_id_to_gpu_handle(D3d12DescriptorHeap* pHeap, DxDescriptorID id)
{
	return { pHeap->start_gpu_handle.ptr + id * pHeap->descriptor_size };
}

function void copy_descriptor_handle(D3d12DescriptorHeap* pSrcHeap, DxDescriptorID srcId, D3d12DescriptorHeap* pDstHeap, DxDescriptorID dstId)
{
	Assert(pSrcHeap->type == pDstHeap->type);
	D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = descriptor_id_to_cpu_handle(pSrcHeap, srcId);
	D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = descriptor_id_to_cpu_handle(pDstHeap, dstId);
	pSrcHeap->device->CopyDescriptorsSimple(1, dstHandle, srcHandle, pSrcHeap->type);
}
/************************************************************************/
// Multi GPU Helper Functions
/************************************************************************/
U32 util_calculate_shared_node_mask(D3d12GpuContext* pRenderer)
{
	if (pRenderer->gpu_mode == kGpuMode_Linked)
		return (1 << pRenderer->linked_node_count) - 1;
	else
		return 0;
}

U32 util_calculate_node_mask(D3d12GpuContext* pRenderer, U32 i)
{
	if (pRenderer->gpu_mode == kGpuMode_Linked)
		return (1 << i);
	else
		return 0;
}
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
