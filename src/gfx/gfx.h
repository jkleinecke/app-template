#ifndef GFX_H
#define GFX_H

// This interface and backing implementation is adapted to from The-Forge
// License and copyright included below

/*
 * Copyright (c) 2017-2022 The Forge Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

#include "tinyimageformat.h"

#ifdef __INTELLISENSE__
// IntelliSense is the code completion engine in Visual Studio. When it parses the source files, __INTELLISENSE__ macro is defined.
// Here we trick IntelliSense into thinking that the renderer functions are not function pointers, but just regular functions.
// What this achieves is filtering out duplicated function names from code completion results and improving the code completion for function parameters.
// This dramatically improves the quality of life for Visual Studio users.
#define GFX_API_FUNCTION(ret, name, ...)                     \
	ret name(__VA_ARGS__);
#else
#define GFX_API_FUNCTION(ret, name, ...)   \
    typedef ret(* Glue(PFN_,name))(__VA_ARGS__); \
    Glue(PFN_,name)    name
#endif


typedef void* GfxContext;
typedef void* GfxFence;
typedef void* GfxSemaphore;
typedef void* GfxQueue;
typedef void* GfxSwapChain;
typedef void* GfxCommandPool;
typedef void* GfxCommand;
typedef void* GfxTexture;
typedef void* GfxBuffer;
typedef void* GfxRenderTarget;
typedef void* GfxSampler;
typedef void* GfxShaderProgram;
typedef void* GfxRootSignature;
typedef void* GfxPipeline;
typedef void* GfxPipelineCache;
typedef void* GfxDescriptorSet;
typedef void* GfxCommandSignature;
typedef void* GfxQueryPool;

enum GfxMarkerType
{

};

struct GfxQueryDesc
{

};

struct GfxQueryPoolDesc
{

};

struct GfxCommandSignatureDesc
{

};

enum GfxFenceStatus
{

};

struct GfxPresentDesc
{

};

struct GfxQueueSubmitDesc
{

};

struct GfxBufferBarrier
{

};

struct GfxTextureBarrier
{

};

struct GfxRenderTargetBarrier
{

};

enum GfxShadingRateCombiner
{

};

enum GfxShadingRate
{

};

struct GfxLoadActionDesc
{

};

struct GfxDescriptorData
{

};

struct GfxDescriptorSetDesc
{

};

struct GfxPipelineCacheDesc
{

};

struct GfxPipelineDesc
{

};

struct GfxRootSignatureDesc
{

};

struct GfxShaderProgramDesc
{

};

struct GfxSamplerDesc
{

};

struct GfxRenderTargetDesc
{

};

struct GfxSubResourceDataDesc
{

};

struct GfxReadRange
{

};

struct GfxTextureDesc
{

};

struct GfxBufferDesc
{

};

struct GfxCommandDesc
{

};

struct GfxCommandPoolDesc
{

};

struct GfxSwapChainDesc
{

};

struct GfxQueueDesc
{

};

struct GfxContextDesc
{

};

// TODO(james): Figure out the memory allocation scheme for the Gfx API
function GfxContext gfx_init_context(GfxContextDesc* desc);

struct GfxApi
{
    GFX_API_FUNCTION(GfxFence, addFence, GfxContext gc);
    GFX_API_FUNCTION(void, removeFence, GfxContext gc, GfxFence fence);

    GFX_API_FUNCTION(GfxSemaphore, addSemaphore, GfxContext gc);
    GFX_API_FUNCTION(void, removeSemaphore, GfxContext gc, GfxSemaphore semaphore);

    GFX_API_FUNCTION(GfxQueue, addQueue, GfxContext gc, GfxQueueDesc* desc);
    GFX_API_FUNCTION(void, removeQueue, GfxContext gc, GfxQueue queue);

    GFX_API_FUNCTION(GfxSwapChain, addSwapChain, GfxContext gc, GfxSwapChainDesc* desc);
    GFX_API_FUNCTION(void, removeSwapChain, GfxContext gc, GfxSwapChain swapchain);

    // command pool operations
    GFX_API_FUNCTION(GfxCommandPool, addCommandPool, GfxContext gc, GfxCommandPoolDesc* desc);
    GFX_API_FUNCTION(void, removeCommandPool, GfxContext gc, GfxCommandPool commandpool);
    GFX_API_FUNCTION(GfxCommand, addCommand, GfxContext gc, GfxCommandPoolDesc* desc);
    GFX_API_FUNCTION(void, removeCommand, GfxContext gc, GfxCommand cmd);
    GFX_API_FUNCTION(void, addCommands, GfxContext gc, GfxCommandDesc* desc, U32 count, GfxCommand* out_commands);
    GFX_API_FUNCTION(void, removeCommands, GfxContext gc, U32 count, GfxCommand* commands);

    //
    //  All buffer, texture loading handled by the resource system -> GfxResourceLoader
    //
    GFX_API_FUNCTION(GfxBuffer, addBuffer, GfxContext gc, GfxBufferDesc* desc);
    GFX_API_FUNCTION(void, removeBuffer, GfxContext gc, GfxBuffer buffer);
    GFX_API_FUNCTION(void, mapBuffer, GfxContext gc, GfxBuffer buffer, GfxReadRange* range);
    GFX_API_FUNCTION(void, unmapBuffer, GfxContext gc, GfxBuffer buffer);
    GFX_API_FUNCTION(void, cmdUpdateBuffer, GfxCommand cmd, GfxBuffer buffer, U64 dstOffset, GfxBuffer srcBuffer, U64 srcOffset, U64 size);
    GFX_API_FUNCTION(void, cmdUpdateSubresource, GfxCommand cmd, GfxTexture texture, GfxBuffer srcBuffer, GfxSubResourceDataDesc* desc);
    GFX_API_FUNCTION(void, cmdCopySubresource, GfxCommand cmd, GfxBuffer dstBuffer, GfxTexture texture, GfxSubResourceDataDesc* desc);
    GFX_API_FUNCTION(GfxTexture, addTexture, GfxContext gc, GfxTextureDesc* desc);
    GFX_API_FUNCTION(void, removeTexture, GfxContext gc, GfxTexture texture);
    GFX_API_FUNCTION(void, addVirtualTexture, GfxCommand cmd, GfxTextureDesc* desc, GfxTexture textures, void* image_data);
    //

    GFX_API_FUNCTION(GfxRenderTarget, addRenderTarget, GfxContext gc, GfxRenderTargetDesc* desc);
    GFX_API_FUNCTION(void, removeRenderTarget, GfxContext gc, GfxRenderTarget rendertarget);
    GFX_API_FUNCTION(GfxSampler, addSampler, GfxContext gc, GfxSamplerDesc* desc);
    GFX_API_FUNCTION(void, removeSampler, GfxContext gc, GfxSampler sampler);

    // shader operations
    GFX_API_FUNCTION(GfxShaderProgram, addShaderBinary, GfxContext gc, GfxShaderProgramDesc* desc);
    GFX_API_FUNCTION(void, removeShader, GfxContext gc, GfxShaderProgram shader);

    GFX_API_FUNCTION(GfxRootSignature, addRootSignature, GfxContext gc, GfxRootSignatureDesc* desc);
    GFX_API_FUNCTION(void, removeRootSignature, GfxContext gc, GfxRootSignature rootsignature);

    // pipeline operations
    GFX_API_FUNCTION(GfxPipeline, addPipeline, GfxContext gc, GfxPipelineDesc* desc);
    GFX_API_FUNCTION(void, removePipeline, GfxContext gc, GfxPipeline pipeline);
    GFX_API_FUNCTION(GfxPipelineCache, addPipelineCache, GfxContext gc, GfxPipelineCacheDesc* desc);
    GFX_API_FUNCTION(void, getPipelineCacheData, GfxContext gc, GfxPipelineCache pipelinecache, U64* out_size, void* out_data);
    GFX_API_FUNCTION(void, removePipelineCache, GfxContext gc, GfxPipelineCache pipelinecache);

    // descriptor set operations
    GFX_API_FUNCTION(GfxDescriptorSet, addDescriptorSet, GfxContext gc, GfxDescriptorSetDesc* desc);
    GFX_API_FUNCTION(void, removeDescriptorSet, GfxContext gc, GfxDescriptorSet descriptorset);
    GFX_API_FUNCTION(void, updateDescriptorSet, GfxContext gc, U32 index, GfxDescriptorSet descriptorset, U32 count, GfxDescriptorData* params);

    // command buffer operations
    GFX_API_FUNCTION(void, resetCommandPool, GfxContext gc, GfxCommandPool commandpool);
    GFX_API_FUNCTION(void, beginCommand, GfxCommand cmd);
    GFX_API_FUNCTION(void, endCommand, GfxCommand cmd);
    GFX_API_FUNCTION(void, cmdBindRenderTargets, GfxCommand cmd, U32 render_target_count, GfxRenderTarget* renderTargets, GfxRenderTarget depth_stencil, GfxLoadActionDesc* loadActions, U32* color_array_slices, U32* color_mip_slices, U32 depth_array_slice, U32 depth_mip_slice);
    GFX_API_FUNCTION(void, cmdSetShadingRate, GfxCommand cmd, GfxShadingRate shading_rate, GfxTexture texture, GfxShadingRateCombiner post_rasterizer_rate, GfxShadingRateCombiner final_rate);
    GFX_API_FUNCTION(void, cmdSetViewport, GfxCommand cmd, F32 x, F32 y, F32 width, F32 height, F32 min_depth, F32 max_depth);
    GFX_API_FUNCTION(void, cmdSetScissor, GfxCommand cmd, U32 x, U32 y, U32 width, U32 height);
    GFX_API_FUNCTION(void, cmdSetStencilReferenceValue, GfxCommand cmd, U32 val);
    GFX_API_FUNCTION(void, cmdBindPipeline, GfxCommand cmd, GfxPipeline pipeline);
    GFX_API_FUNCTION(void, cmdBindDescriptorSet, GfxCommand cmd, U32 index, GfxDescriptorSet descriptorset);
    GFX_API_FUNCTION(void, cmdBindPushConstants, GfxCommand cmd, GfxRootSignature rootsignature, U32 index, void* constant_data);
    GFX_API_FUNCTION(void, cmdBindDescriptorSetWithRootCbvs, GfxCommand cmd, U32 index, GfxDescriptorSet descriptorset, U32 count, GfxDescriptorData* params);
    GFX_API_FUNCTION(void, cmdBindIndexBuffer, GfxCommand cmd, GfxBuffer buffer, U32 index_type, U64 offset);
    GFX_API_FUNCTION(void, cmdBindVertexBuffer, GfxCommand cmd, U32 count, GfxBuffer* buffers, U32* strides, U64* offsets);
    GFX_API_FUNCTION(void, cmdDraw, GfxCommand cmd, U32 vertex_count, U32 first_vertex);
    GFX_API_FUNCTION(void, cmdDrawInstanced, GfxCommand cmd, U32 vertex_count, U32 first_vertex, U32 instance_count, U32 first_instance);
    GFX_API_FUNCTION(void, cmdDrawIndexed, GfxCommand cmd, U32 index_count, U32 first_index, U32 first_vertex);
    GFX_API_FUNCTION(void, cmdDrawIndexedInstanced, GfxCommand cmd, U32 index_count, U32 first_index, U32 instance_count, U32 first_vertex, U32 first_instance);
    GFX_API_FUNCTION(void, cmdDispatch, GfxCommand cmd, U32 group_count_x, U32 group_count_y, U32 group_count_z);

    // transition commands
    GFX_API_FUNCTION(void, cmdResourceBarrier, GfxCommand cmd, U32 buffer_barrier_count, GfxBufferBarrier* buffer_barriers, U32 texture_barrier_count, GfxTextureBarrier* texture_barriers, U32 rendertarget_barrier_count, GfxRenderTargetBarrier* rendertarget_barriers);

    // virtual textures
    GFX_API_FUNCTION(void, cmdUpdateVirtualTexture, GfxCommand cmd, GfxTexture texture, U32 current_image);

    // queue/fence/swapchain operations
    GFX_API_FUNCTION(void, acquireNextImage, GfxContext gc, GfxSwapChain swapchain, GfxSemaphore signal_semaphore, GfxFence fence, U32* out_image_index);
    GFX_API_FUNCTION(void, queueSubmit, GfxQueue queue, GfxQueueSubmitDesc* desc);
    GFX_API_FUNCTION(void, queuePresent, GfxQueue queue, GfxQueuePresentDesc* desc);
    GFX_API_FUNCTION(void, waitQueueIdle, GfxQueue queue);
    GFX_API_FUNCTION(GfxFenceStatus, getFenceStatus, GfxContext gc, GfxFence fence);
    GFX_API_FUNCTION(void, waitForFences, GfxContext gc, U32 count, GfxFence* fences);
    GFX_API_FUNCTION(void, toggleVSync, GfxContext gc, GfxSwapChain swapchain);

    //Returns the recommended format for the swapchain.
    //If true is passed for the hintHDR parameter, it will return an HDR format IF the platform supports it
    //If false is passed or the platform does not support HDR a non HDR format is returned.
    //If true is passed for the hintSrgb parameter, it will return format that is will do gamma correction automatically
    //If false is passed for the hintSrgb parameter the gamma correction should be done as a postprocess step before submitting image to swapchain
    GFX_API_FUNCTION(TinyImageFormat, getRecommendedSwapchainFormat, bool hintHDR, bool hintSRGB)

    // indirect draw operations
    GFX_API_FUNCTION(GfxCommandSignature, addIndirectCommandSignature, GfxContext gc, GfxCommandSignatureDesc* desc);
    GFX_API_FUNCTION(void, removeIndirectCommandSignature, GfxContext gc, GfxCommandSignature cmdsignature);
    GFX_API_FUNCTION(void, cmdExecuteIndirect, GfxCommand cmd, GfxCommandSignature cmdsignature, U32 max_cmd_count, GfxBuffer indirect_buffer, U64 buffer_offset, GfxBuffer counter_buffer, U64 counter_buffer_offset);

    //
    // GPU Query Interface
    //
    GFX_API_FUNCTION(F64, getTimestampFrequency, GfxQueue queue);
    GFX_API_FUNCTION(GfxQueryPool, addQueryPool, GfxContext gc, GfxQueryPoolDesc* desc);
    GFX_API_FUNCTION(void, removeQueryPool, GfxContext gc, GfxQueryPool querypool);
    GFX_API_FUNCTION(void, cmdResetQueryPool, GfxCommand cmd, GfxQueryPool querypool, U32 start_query, U32 query_count);
    GFX_API_FUNCTION(void, cmdBeginQuery, GfxCommand cmd, GfxQueryPool querypool, GfxQueryDesc* desc);
    GFX_API_FUNCTION(void, cmdEndQuery, GfxCommand cmd, GfxQueryPool querypool, GfxQueryDesc* desc);
    GFX_API_FUNCTION(void, cmdResolveQuery, GfxCommand cmd, GfxQueryPool querypool, GfxBuffer readback_buffer, U32 start_query, U32 query_count);
    //
    // Stats Info Interface
    //
    GFX_API_FUNCTION(void, calculateMemoryStats, MemArena* arena, GfxContext gc, String8List* out_stats);
    GFX_API_FUNCTION(void, calculateMemoryUse, GfxContext gc, U64* out_usedbytes, U64* out_totalallocatedbytes);
    //
    // Debug Marker Interface
    //
    GFX_API_FUNCTION(void, cmdAddDebugMarker, GfxCommand cmd, F32 r, F32 g, F32 b, String8 name);
    GFX_API_FUNCTION(void, cmdBeginDebugMarker, GfxCommand cmd, F32 r, F32 g, F32 b, String8 name);
    GFX_API_FUNCTION(void, cmdEndDebugMarker, GfxCommand cmd);
    GFX_API_FUNCTION(U32, cmdWriteMarker, GfxCommand cmd, GfxMarkerType marker_type, U32 marker_value, GfxBuffer buffer, U64 offset, B32 use_auto_flags);
    //
    // Resource Debug Naming Interface
    //
    GFX_API_FUNCTION(void, setBufferName, GfxContext gc, GfxBuffer buffer, String8 name);
    GFX_API_FUNCTION(void, setTextureName, GfxContext gc, GfxTexture texture, String8 name);
    GFX_API_FUNCTION(void, setRenderTargetName, GfxContext gc, GfxRenderTarget rendertarget, String8 name);
    GFX_API_FUNCTION(void, setPipelineName, GfxContext gc, GfxPipeline pipeline, String8 name);
    //
    GFX_API_FUNCTION(U32, getDescriptorIndexFromName, GfxRootSignature rootsignature, String8 name);
};

#endif
