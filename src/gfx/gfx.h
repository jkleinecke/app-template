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

#if OS_WINDOWS
//# define DX11 1
# define DX12   1
// # define VULKAN 1
#elif OS_MACOS
# define METAL 1
#elif OS_LINUX
# define VULKAN 1
#endif

#if !defined(DX11)
# define DX11 0
#endif
#if !defined(DX12)
# define DX12 0
#endif
#if !defined(VULKAN)
# define VULKAN 0
#endif
#if !defined(METAL)
# define METAL 0
#endif

enum
{
	kMax_InstanceExtensions = 64,
	kMax_DeviceExtensions = 64,
	/// Max number of GPUs in SLI or Cross-Fire
	kMax_LinkedGpus = 4,
	/// Max number of GPUs in unlinked mode
	kMax_UnlinkedGpus = 4,
	/// Max number of GPus for either linked or unlinked mode.
	kMax_MultipleGpus = 4,
	kMax_RenderTargetAttachments = 8,
	kMax_VertexBindings = 15,
	kMax_VertexAttribs = 15,
	kMax_ResourceNameLength = 256,
	kMax_SemanticNameLength = 128,
	kMax_DebugNameLength = 128,
	kMax_MipLevels = 0xFFFFFFFF,
	kMax_SwapchainImages = 3,
	kMax_GpuVendorStringLength = 256,    //max size for GPUVendorPreset strings
#if VULKAN
	MAX_PLANE_COUNT = 3,
	MAX_DESCRIPTOR_POOL_SIZE_ARRAY_COUNT = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT + 1,
#endif
};

// Raytracing
typedef void* GfxRaytracing;
struct GfxAccelerationStructure;    // TODO(james): Is this only valid for DX12?

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
typedef void* GfxShader;
typedef void* GfxRootSignature;
typedef void* GfxPipeline;
typedef void* GfxPipelineCache;
typedef void* GfxDescriptorSet;
typedef void* GfxCommandSignature;
typedef void* GfxQueryPool;

enum GfxMarkerType
{
    kMarkerType_Default = 0x0,
    kMarkerType_In = 0x1,
    kMarkerType_Out = 0x2,
    kMarkerType_InOut = 0x3,
};

enum GfxFenceStatus
{
    kFenceStatus_Complete = 0,
    kFenceStatus_Incomplete,
    kFenceStatus_NotSubmitted,
};

enum GfxShadingRateCombiner
{
    kShadingRateCombiner_PassThrough = 0,
    kShadingRateCombiner_Override = 1,
    kShadingRateCombiner_Min = 2,
    kShadingRateCombiner_Max = 3,
    kShadingRateCombiner_Sum = 4,
};

enum GfxShadingRate
{
    kShadingRate_NotSupported = 0x0,
    kShadingRate_Full = 0x1,
    kShadingRate_Half = kShadingRate_Full << 1,
    kShadingRate_Quarter = kShadingRate_Half << 1,
    kShadingRate_Eighth = kShadingRate_Quarter << 1,
    kShadingRate_1x2 = kShadingRate_Eighth << 1,
    kShadingRate_2x1 = kShadingRate_1x2 << 1,
    kShadingRate_2x4 = kShadingRate_2x1 << 1,
    kShadingRate_4x2 = kShadingRate_2x4 << 1,
};
enum GfxQueryType
{
    kQueryType_Timestamp = 0,
    kQueryType_PipelineStats,
    kQueryType_Occlusion,
    kQueryType_Count,
};

enum GfxIndirectArgumentType
{
    kIndirectArg_Invalid,
    kIndirectArg_Draw,
    kIndirectArg_DrawIndex,
    kIndirectArg_Dispatch,
    kIndirectArg_VertexBuffer,
    kIndirectArg_IndexBuffer,
    kIndirectArg_Constant,
    kIndirectArg_ConstantBufferView,    // only for dx
    kIndirectArg_ShaderResourceView,    // only for dx
    kIndirectArg_UnorderedAccessView,   // only for dx
    kIndirectArg_CommandBuffer,         // metal ICB
    kIndirectArg_CommandBufferReset,    // metal ICB reset
    kIndirectArg_CommandBufferOptimize, // metal ICB optimization
};

enum GfxResourceState
{
    kResourceState_Undefined                = 0x0,
    kResourceState_VertexAndConstantBuffer  = 0x1,
    kResourceState_IndexBuffer              = 0x2,
    kResourceState_RenderTarget             = 0x4,
    kResourceState_UnorderedAccess          = 0x8,
    kResourceState_DepthWrite               = 0x10,
    kResourceState_DepthRead                = 0x20,
    kResourceState_NonPixelShaderResource   = 0x40,
    kResourceState_PixelShaderResource      = 0x80,
    kResourceState_ShaderResource           = 0x40 | 0x80,
    kResourceState_StreamOut                = 0x100,
    kResourceState_IndirectArg              = 0x200,
    kResourceState_CopyDest                 = 0x400,
    kResourceState_CopySource               = 0x800,
    kResourceState_GenericRead              = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
    kResourceState_Present                  = 0x1000,
    kResourceState_Common                   = 0x2000,
    kResourceState_RayTracingAccelStruct    = 0x4000,
    kResourceState_ShadingRateSource        = 0x8000,   
};

enum GfxLoadActionType
{
    kLoadAction_DontCare,
    kLoadAction_Load,
    kLoadAction_Clear,
};

enum GfxStoreActionType
{
    kStoreAction_Store,
    kStoreAction_DontCare,
    kStoreAction_None,
    // Only used for MSAA resolve attachments
    kStoreAction_ResolveStore,
    kStoreAction_ResolveDontCare,
};

enum GfxDescriptorUpdateFrequency
{
    kDescriptorUpdateFreq_None = 0,
    kDescriptorUpdateFreq_PerFrame,
    kDescriptorUpdateFreq_PerBatch,
    kDescriptorUpdateFreq_PerDraw
};

enum GfxPipelineCacheFlags
{
    kPipelineCacheFlag_None = 0x0,
    kPipelineCacheFlag_ExternallySynced = 0x1,
};

enum GfxPipelineType
{
    kPipeline_Undefined = 0,
    kPipeline_Compute,
    kPipeline_Graphics,
    kPipeline_RayTracing,
};

enum GfxSampleCount
{
    kSampleCount_1 = 1,
    kSampleCount_2 = 2,
    kSampleCount_4 = 4,
    kSampleCount_8 = 8,
    kSampleCount_16 = 16,
};

enum GfxPrimitiveTopology
{
    kPrimitiveTopo_PointList = 0,
    kPrimitiveTopo_LineList,
    kPrimitiveTopo_LineStrip,
    kPrimitiveTopo_TriList,
    kPrimitiveTopo_TriStrip,
    kPrimitiveTopo_PatchList,
};

enum GfxVertexAttribRate
{
    kVertexAttribRate_Vertex = 0,
    kVertexAttribRate_Instance,
};

enum GfxShaderSemantic
{
    kShaderSemantic_Undefined = 0,
    kShaderSemantic_Position,
    kShaderSemantic_Normal,
    kShaderSemantic_Color,
    kShaderSemantic_Tangent,
    kShaderSemantic_Bitangent,
    kShaderSemantic_Joints,
    kShaderSemantic_Weights,
    kShaderSemantic_ShadingRate,
    kShaderSemantic_TexCoord0,
    kShaderSemantic_TexCoord1,
    kShaderSemantic_TexCoord2,
    kShaderSemantic_TexCoord3,
    kShaderSemantic_TexCoord4,
    kShaderSemantic_TexCoord5,
    kShaderSemantic_TexCoord6,
    kShaderSemantic_TexCoord7,
    kShaderSemantic_TexCoord8,
    kShaderSemantic_TexCoord9,
};

enum GfxCullMode
{
    kCull_None = 0,
    kCull_Back,
    kCull_Front,
    kCull_Both,
};

enum GfxFillMode
{
    kFill_Solid = 0,
    kFill_Wireframe,
};

enum GfxFrontFace
{
    kFrontFace_CCW = 0,
    kFrontFace_CW,
};

enum GfxCompareMode
{
    kCompare_Never,
    kCompare_Less,
    kCompare_Equal,
    kCompare_LEqual,
    kCompare_Greater,
    kCompare_NotEqual,
    kCompare_GEqual,
    kCompare_Always,
};

enum GfxStencilOp
{
    kStencilOp_Keep,
    kStencilOp_SetZero,
    kStencilOp_Replace,
    kStencilOp_Invert,
    kStencilOp_Incr,
    kStencilOp_Desc,
    kStencilOp_IncrSat,
    kStencilOp_DecrSat,
};

enum GfxBlendConstant
{
    kBlendConstant_Zero = 0,
    kBlendConstant_One,
    kBlendConstant_SrcColor,
    kBlendConstant_OneMinusSrcColor,
    kBlendConstant_DstColor,
    kBlendConstant_OneMinusDstColor,
    kBlendConstant_SrcAlpha,
    kBlendConstant_OneMinusSrcAlpha,
    kBlendConstant_DstAlpha,
    kBlendConstant_OneMinusDstAlpha,
    kBlendConstant_SrcAlphaSaturate,
    kBlendConstant_BlendFactor,
    kBlendConstant_OneMinusBlendFactor,
};

enum GfxBlendMode
{
    kBlendMode_Add,
    kBlendMode_Subtract,
    kBlendMode_ReverseSubtract,
    kBlendMode_Min,
    kBlendMode_Max,
};

enum GfxBlendStateTargets
{
    kBlendStateTarget_0     = 0x1,
    kBlendStateTarget_1     = 0x2,
    kBlendStateTarget_2     = 0x4,
    kBlendStateTarget_3     = 0x8,
    kBlendStateTarget_4     = 0x10,
    kBlendStateTarget_5     = 0x20,
    kBlendStateTarget_6     = 0x40,
    kBlendStateTarget_7     = 0x80,
    kBlendStateTarget_All   = 0xFF,
};

enum GfxShaderStage
{
    kShaderStage_None   = 0,
    kShaderStage_Vert   = 0x1,
    kShaderStage_Tesc   = 0x2,
    kShaderStage_Tese   = 0x4,
    kShaderStage_Geom   = 0x8,
    kShaderStage_Frag   = 0x10,
    kShaderStage_Comp   = 0x20,
    kShaderStage_Raytracing = 0x40,
    kShaderStage_AllGraphics = (kShaderStage_Vert | kShaderStage_Tesc | kShaderStage_Tese | kShaderStage_Geom | kShaderStage_Frag),
    kShaderStage_Hull   = kShaderStage_Tesc,
    kShaderStage_Domn   = kShaderStage_Tese,
};

enum GfxFilterType
{
    kFilter_Nearest = 0,
    kFilter_Linear,
};

enum GfxMipMapMode
{
    kMipMapMode_Nearest = 0,
    kMipMapMode_Linear,
};

enum GfxAddressMode
{
    kAddressMode_Mirror,
    kAddressMode_Repeat,
    kAddressMode_ClampToEdge,
    kAddressMode_ClampToBorder,
};

enum GfxSamplerModelConversion
{
    kSamplerModelConversion_RgbIdentity = 0,
    kSamplerModelConversion_YcbcrIdentity = 1,
    kSamplerModelConversion_Ycbcr709 = 2,
    kSamplerModelConversion_Ycbcr601 = 3,
    kSamplerModelConversion_Ycbcr2020 = 4,
};

enum GfxSamplerRange
{
    kSamplerRange_Full = 0,
    kSamplerRange_Narrow = 1,
};

enum GfxSampleLocation
{
    kSampleLocation_Cosited = 0,
    kSampleLocation_Midpoint = 1,
};

struct GfxQueryDesc
{
    U32 index;
};

struct GfxQueryPoolDesc
{
    GfxQueryType type;
    U32 query_count;
    U32 node_index;
};

struct GfxIndirectArgument
{
    GfxIndirectArgumentType type;
    U32                     offset;
};

struct GfxIndirectArgumentDescriptor
{
    GfxIndirectArgumentType type;
    U32                     index;
    U32                     byte_size;
};

struct GfxCommandSignatureDesc
{
    GfxRootSignature                root_signature;
    GfxIndirectArgumentDescriptor*  arg_descs;
    U32                             arg_count;
    B32                             packed;     // set to true if indirect arg struct should not be aligned to 16 bytes
};

struct GfxQueuePresentDesc
{
    GfxSwapChain    swapchain;
    GfxSemaphore*   wait_semaphores;
    U32             wait_semaphore_count;
    B32             submit_done;
    U8              index;
};

struct GfxQueueSubmitDesc
{
    GfxCommand*     cmds;
    GfxFence*       fences;
    GfxSemaphore*   wait_semaphores;
    GfxSemaphore*   signal_semaphores;
    U32             cmd_count;
    U32             wait_semaphore_count;
    U32             signal_semaphore_count;
    B32             submit_done;
};

struct GfxBufferBarrier
{
    GfxBuffer           buffer;
    GfxResourceState    current_state;
    GfxResourceState    new_state;
    U8                  begin_only  : 1;
    U8                  end_only    : 1;
    U8                  acquire     : 1;
    U8                  release     : 1;
    U8                  queue_type  : 5;
};

struct GfxTextureBarrier
{
    GfxTexture          texture;
    GfxResourceState    current_state;
    GfxResourceState    new_state;
    U8                  begin_only  : 1;
    U8                  end_only    : 1;
    U8                  acquire     : 1;
    U8                  release     : 1;
    U8                  queue_type  : 5;
    U8                  subresource_barrier : 1;
    // following ignored if subresource barrier is false
    U8                  mip_level   : 7;
    U16                 array_layer;
};

struct GfxRenderTargetBarrier
{
    GfxRenderTarget     rendertarget;
    GfxResourceState    current_state;
    GfxResourceState    new_state;
    U8                  begin_only  : 1;
    U8                  end_only    : 1;
    U8                  acquire     : 1;
    U8                  release     : 1;
    U8                  queue_type  : 5;
    U8                  subresource_barrier : 1;
    // following ignored if subresource barrier is false
    U8                  mip_level   : 7;
    U16                 array_layer;
};

union GfxClearValue
{
    struct
    {
        F32 r, g, b, a;
    };
    struct
    {
        F32 depth;
        U32 stencil;
    };
};

struct GfxLoadActionsDesc
{
    GfxLoadActionType   load_action_color[kMax_RenderTargetAttachments];
    GfxLoadActionType   load_action_depth;
    GfxLoadActionType   load_action_stencil;
    GfxClearValue       clear_color_values[kMax_RenderTargetAttachments];
    GfxClearValue       clear_depth;
    GfxStoreActionType  store_action_color[kMax_RenderTargetAttachments];
    GfxStoreActionType  store_action_depth;
    GfxStoreActionType  store_action_stencil;
};

struct GfxDescriptorDataRange
{
	U32 offset;
	U32 size;
};

struct GfxDescriptorData
{
    /// User can either set name of descriptor or index (index in pRootSignature->pDescriptors array)
	/// Name of descriptor
	const char* name;
	/// Number of array entries to update (array size of ppTextures/ppBuffers/...)
	U32         count : 31;
	/// Dst offset into the array descriptor (useful for updating few entries in a large array)
    // Example: to update 6th entry in a bindless texture descriptor, array_offset will be 6 and count will be 1)
	U32         array_offset : 20;
	// Index in pRootSignature->pDescriptors array - Cache index using getDescriptorIndexFromName to avoid using string checks at runtime
	U32         index : 10;
	U32         bind_by_index : 1;
	
	// Range to bind (buffer offset, size)
	GfxDescriptorDataRange* ranges;

	// Binds stencil only descriptor instead of color/depth
	bool bind_stencil_resource : 1;

	union
	{
		struct
		{
			// When binding UAV, control the mip slice to to bind for UAV (example - generating mipmaps in a compute shader)
			U16                 uav_mip_slice;
			// Binds entire mip chain as array of UAV
			bool                bind_mip_chain;
		};
		struct
		{
			// Bind MTLIndirectCommandBuffer along with the MTLBuffer
			const char*         icb_name;
			uint32_t            icb_index;
			bool                bind_icb;
		};
	};
	/// Array of resources containing descriptor handles or constant to be used in ring buffer memory - DescriptorRange can hold only one resource type array
	union
	{
		/// Array of texture descriptors (srv and uav textures)
		GfxTexture*               textures;
		/// Array of sampler descriptors
		GfxSampler*               samplers;
		/// Array of buffer descriptors (srv, uav and cbv buffers)
		GfxBuffer*                buffers;
		/// Custom binding (raytracing acceleration structure ...)
		GfxAccelerationStructure** accelerationStructures;
	};
};

struct GfxDescriptorSetDesc
{
    GfxRootSignature                rootsignature;
    GfxDescriptorUpdateFrequency    update_frequency;
    U32                             max_sets;
    U32                             node_index;
};

struct GfxPipelineCacheDesc
{
    // initial pipeline cache data (can be NULL which means empty pipeline cache)
    void*                   data;
    // initial size
    U64                     size;
    GfxPipelineCacheFlags   flags;
};

struct GfxComputePipelineDesc
{
    GfxShader        shader_program;
    GfxRootSignature root_signature;
};

struct GfxVertexAttrib
{
    GfxShaderSemantic   semantic;
    U32                 semantic_name_length;
    char                semantic_name[kMax_SemanticNameLength];
    TinyImageFormat     format;
    U32                 binding;
    U32                 location;
    U32                 offset;
    GfxVertexAttribRate rate;
};

struct GfxVertexLayout
{
    U32             attrib_count;
    GfxVertexAttrib attribs[kMax_VertexAttribs];
    U32             strides[kMax_VertexAttribs];
};

struct GfxBlendStateDesc
{
    GfxBlendConstant        src_factors[kMax_RenderTargetAttachments];
    GfxBlendConstant        dst_factors[kMax_RenderTargetAttachments];
    GfxBlendConstant        src_alpha_factors[kMax_RenderTargetAttachments];
    GfxBlendConstant        dst_alpha_factors[kMax_RenderTargetAttachments];
    GfxBlendMode            blend_modes[kMax_RenderTargetAttachments];
    GfxBlendMode            blend_alpha_modes[kMax_RenderTargetAttachments];
    S32                     masks[kMax_RenderTargetAttachments];
    GfxBlendStateTargets    render_target_mask;
    B32                     enable_alpha_to_coverage;
    B32                     enable_independent_blend;
};

struct GfxDepthStateDesc
{
    B32             enable_depth_test;
    B32             enable_depth_write;
    GfxCompareMode  depth_func;
    B32             enable_stencil_test;
    U8              stencil_read_mask;
    U8              stencil_write_mask;
    GfxCompareMode  stencil_front_func;
    GfxStencilOp    stencil_front_fail;
    GfxStencilOp    depth_front_fail;
    GfxStencilOp    stencil_front_pass;
    GfxCompareMode  stencil_back_func;
    GfxStencilOp    stencil_back_fail;
    GfxStencilOp    depth_back_fail;
    GfxStencilOp    stencil_back_pass;
};

struct GfxRasterizerStateDesc
{
    GfxCullMode     cull_mode;
    S32             depth_bias;
    F32             slope_scaled_depth_bias;
    GfxFillMode     fill_mode;
    GfxFrontFace    front_face;
    B32             enable_multisample;
    B32             enable_scissor;
    B32             enable_depth_clamp;
};

struct GfxGraphicsPipelineDesc
{
    GfxShader                   shader_program;
    GfxRootSignature            root_signature;
    GfxVertexLayout*            vertex_layout;
    GfxBlendStateDesc*          blend_state;
    GfxDepthStateDesc*          depth_state;
    GfxRasterizerStateDesc*     rasterizer_state;
    TinyImageFormat*            color_formats;
    GfxStoreActionType*         color_resolve_actions;  // only for MSAA resolve attachments
    U32                         render_target_count;
    GfxSampleCount              sample_count;
    TinyImageFormat             depth_stencil_format;
    GfxPrimitiveTopology        primitive_topology;
    B32                         enable_support_indirect_cmd_buffer;
    B32                         enable_vr_foveated_rendering;
};

/************************************************************************/
// Defines which shaders will be used by this hit group
// #intersection_shader - Name of shader used to test intersection with ray
//	  This will be NULL as long as user does not specify ACCELERATION_STRUCTURE_GEOMETRY_TYPE_PROCEDURAL_PRIMITIVE_AABBS
// #any_hit_shader - Name of shader executed when a ray hits
//	  This will be NULL if user specifies ACCELERATION_STRUCTURE_GEOMETRY_FLAG_OPAQUE in geometry creation
//	  This shader is usually used for work like alpha testing geometry to see if an intersection is valid
// #closest_hit_shader - Name of shader executed for the intersection closest to the ray
//	  This shader will do most of the work like shading
// #hit_group_name - User defined name of the hit group. Use the same name when creating the shader table
/************************************************************************/
struct GfxRaytracingHitGroup
{
	GfxRootSignature root_signature;
	GfxShader       intersection_shader;
	GfxShader       any_hit_shader;
	GfxShader       closest_hit_shader;
	String8          hit_group_name;
};

/************************************************************************/
// #global_root_signature - Root Signature used by all shaders in the ppShaders array
// #miss_shaders - Array of all shaders which can be called during the raytracing operation
//	  This includes the ray generation shader, all miss, any hit, closest hit shaders
// #hit_groups - Name of the hit groups which will tell the pipeline about which combination of hit shaders to use
// #payload_size - Size of the payload struct for passing data to and from the shaders.
//	  Example - float4 payload sent by raygen shader which will be filled by miss shader as a skybox color
//				  or by hit shader as shaded color
// #attribute_size - Size of the intersection attribute. As long as user uses the default intersection shader
//	  this size is sizeof(float2) which represents the ZW of the barycentric co-ordinates of the intersection
/************************************************************************/
struct GfxRaytracingPipelineDesc
{
    GfxRaytracing           raytracing;
    GfxRootSignature        global_root_signature;
    GfxShader               ray_gen_shader;
    GfxRootSignature        ray_gen_root_signature;
    GfxShader*              miss_shaders;
    GfxRootSignature*       miss_root_signatures;
    GfxRaytracingHitGroup*  hit_groups;
    GfxRootSignature        empty_root_signature;
    U32                     miss_shader_count;
    U32                     hit_group_count;
    U32                     payload_size;
    U32                     attribute_size;
    U32                     max_trace_recursion_depth;
    U32                     max_rays_count;
};

struct GfxPipelineDesc
{
    union
    {
        GfxComputePipelineDesc compute_desc;
        GfxGraphicsPipelineDesc graphics_desc;
        GfxRaytracingPipelineDesc raytracing_desc;
    };
    GfxPipelineCache    cache;
    void*               pipeline_extensions;
    String8             name;
    GfxPipelineType     type;
    U32                 extension_count;
};

enum GfxRootSignatureFlags
{
    kRootSignatureFlag_None = 0,
    kRootSignatureFlag_LocalBit = 0x1,
};

struct GfxRootSignatureDesc
{
    GfxShader*              shaders;
    U32                     shader_count;
    U32                     max_bindless_textures;
    const char**            static_sampler_names;   // TODO(james): Should this be String8 instead?
    GfxSampler*             static_samplers;
    U32                     static_sampler_count;                         
    GfxRootSignatureFlags   flags;
};

struct GfxShaderStageDesc
{
    void*       byte_code;
    U32         byte_code_size;
    const char* entry_point;    // TODO(james): Should be String8 instead
};

struct GfxShaderConstant
{
    void*   value;
    U32     index;
    U32     size;
};

struct GfxShaderDesc
{
    GfxShaderStage          stages;
    U32                     owns_byte_code : 1; // TODO(james): is this necessary?
    GfxShaderStageDesc      vert;
    GfxShaderStageDesc      frag;
    GfxShaderStageDesc      geom;
    GfxShaderStageDesc      hull;
    GfxShaderStageDesc      domain;
    GfxShaderStageDesc      comp;
    GfxShaderConstant*      constants;
    U32                     constant_count;
};

struct GfxSamplerDesc
{
    GfxFilterType       min_filter;
    GfxFilterType       mag_filter;
    GfxMipMapMode       mip_map_mode;
    GfxAddressMode      address_u;
    GfxAddressMode      address_v;
    GfxAddressMode      address_w;
    F32                 mip_lod_bias;
    B32                 enable_lod_range;
    F32                 min_lod;
    F32                 max_lod;
    F32                 max_anisotropy;
    GfxCompareMode      compare_func;

#if VULKAN
    struct
    {
        TinyImageFormat             format;
        GfxSamplerModelConversion   model;
        GfxSamplerRange             range;
        GfxSampleLocation           chroma_offset_x;
        GfxSampleLocation           chroma_offset_y;
        GfxFilterType               chroma_filter;
        B32                         force_explicit_reconstruction;
    } sampler_conversion_desc;
#endif
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
    GFX_API_FUNCTION(GfxShader, addShaderBinary, GfxContext gc, GfxShaderDesc* desc);
    GFX_API_FUNCTION(void, removeShader, GfxContext gc, GfxShader shader);

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
    GFX_API_FUNCTION(void, cmdBindRenderTargets, GfxCommand cmd, U32 render_target_count, GfxRenderTarget* renderTargets, GfxRenderTarget depth_stencil, GfxLoadActionsDesc* loadActions, U32* color_array_slices, U32* color_mip_slices, U32 depth_array_slice, U32 depth_mip_slice);
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
