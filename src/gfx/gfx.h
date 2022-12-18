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

#define GFX_USE_MULTIPLE_RENDER_APIS 0
#define GFX_ENABLE_INSIGHT_AFTERMATH 0

#if OS_WINDOWS
//# define DX11 1
# define DX12   1
// # define VULKAN 1
// # define GFX_USE_MULTIPLE_RENDER_APIS
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
#if !defined(GLES)
# define GLES 0
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


typedef void* GfxGpuContext;
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

// Raytracing
typedef void* GfxRaytracing;
struct GfxAccelerationStructure;    // TODO(james): Is this only valid for DX12?

// TODO(james): abstract this out into a more general application logging system
enum GfxLogLevel
{
    kLog_None        = 0,
    kLog_Raw         = 0x1,
    kLog_Debug       = 0x2,
    kLog_Info        = 0x4,
    kLog_Warning     = 0x8,
    kLog_Error       = 0x10,
    kLog_All         = 0xFF,
};

enum GfxGpuMode
{
    kGpuMode_Single = 0,
    kGpuMode_Linked,
    kGpuMode_Unlinked,
};

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
    kDescriptorUpdateFreq_PerDraw,
    kDescriptorUpdateFreq_COUNT,
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
    kCull_COUNT,
};

enum GfxFillMode
{
    kFill_Solid = 0,
    kFill_Wireframe,
    kFill_COUNT,
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
    kCompare_COUNT,
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
    kStencilOp_COUNT,
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
    kBlendConstant_COUNT,
};

enum GfxBlendMode
{
    kBlendMode_Add,
    kBlendMode_Subtract,
    kBlendMode_ReverseSubtract,
    kBlendMode_Min,
    kBlendMode_Max,
    kBlendMode_COUNT,
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

enum GfxTextureCreationFlags
{
    /// Default flag (Texture will use default allocation strategy decided by the api specific allocator)
    kTextureCreationFlag_None = 0,
    /// Texture will allocate its own memory (COMMITTED resource)
    kTextureCreationFlag_OwnMemoryBit = 0x01,
    /// Texture will be allocated in memory which can be shared among multiple processes
    kTextureCreationFlag_ExportBit = 0x02,
    /// Texture will be allocated in memory which can be shared among multiple gpus
    kTextureCreationFlag_ExportAdapterBit = 0x04,
    /// Texture will be imported from a handle created in another process
    kTextureCreationFlag_ImportBit = 0x08,
    /// Use ESRAM to store this texture
    kTextureCreationFlag_Esram = 0x10,
    /// Use on-tile memory to store this texture
    kTextureCreationFlag_OnTile = 0x20, 
    /// Prevent compression meta data from generating (XBox)
    kTextureCreationFlag_NoCompression = 0x40,
    /// Force 2D instead of automatically determining dimension based on width, height, depth
    kTextureCreationFlag_Force2d = 0x80,
    /// Force 3D instead of automatically determining dimension based on width, height, depth
    kTextureCreationFlag_Force3d = 0x100,
    /// Display target
    kTextureCreationFlag_AllowDisplayTarget = 0x200,
    /// Create an sRGB texture.
    kTextureCreationFlag_SRGB = 0x400,
    /// Create a normal map texture
    kTextureCreationFlag_NormalMap = 0x800,
    /// Fast clear
    kTextureCreationFlag_FastClear = 0x1000,
    /// Fragment mask
    kTextureCreationFlag_FragMask = 0x2000,
    /// Doubles the amount of array layers of the texture when rendering VR. Also forces the texture to be a 2D Array texture.
    kTextureCreationFlag_VrMultiview = 0x4000,
    /// Binds the FFR fragment density if this texture is used as a render target.
    kTextureCreationFlag_VrFoveatedRendering = 0x8000,
    /// Creates resolve attachment for auto resolve (MSAA on tiled architecture - Resolve can be done on tile through render pass)
    kTextureCreationFlag_CreateResolveAttachment = 0x10000,
};

enum  GfxDescriptorType
{
    kDescriptorType_Undefined = 0,
    kDescriptorType_Sampler = 0x01,
    /// SRV Read only texture
    kDescriptorType_Texture = (kDescriptorType_Sampler << 1),
    /// UAV Texture
    kDescriptorType_RWTexture = (kDescriptorType_Texture << 1),
    /// SRV Read only buffer
    kDescriptorType_Buffer = (kDescriptorType_RWTexture << 1),
    kDescriptorType_BufferRaw = (kDescriptorType_Buffer | (kDescriptorType_Buffer << 1)),
    /// UAV Buffer
    kDescriptorType_RWBuffer = (kDescriptorType_Buffer << 2),
    kDescriptorType_RWBufferRaw = (kDescriptorType_RWBuffer | (kDescriptorType_RWBuffer << 1)),
    /// Uniform Buffer
    kDescriptorType_UniformBuffer = (kDescriptorType_RWBuffer << 2),
    /// Push constant / Root constant
    kDescriptorType_RootConstant = (kDescriptorType_UniformBuffer << 1),
    /// IA
    kDescriptorType_VertexBuffer = (kDescriptorType_RootConstant << 1),
    kDescriptorType_IndexBuffer = (kDescriptorType_VertexBuffer << 1),
    kDescriptorType_IndirectBuffer = (kDescriptorType_IndexBuffer << 1),
    /// Cubemap SRV
    kDescriptorType_TextureCube = (kDescriptorType_Texture | (kDescriptorType_IndirectBuffer << 1)),
    /// RTV / DSV per mip slice
    kDescriptorType_RenderTargetMipSlices = (kDescriptorType_IndirectBuffer << 2),
    /// RTV / DSV per array slice
    kDescriptorType_RenderTargetArraySlices = (kDescriptorType_RenderTargetMipSlices << 1),
    /// RTV / DSV per depth slice
    kDescriptorType_RenderTargetDepthSlices = (kDescriptorType_RenderTargetArraySlices << 1),
    kDescriptorType_RayTracing = (kDescriptorType_RenderTargetDepthSlices << 1),
    kDescriptorType_IndirectCommandBuffer = (kDescriptorType_RayTracing << 1),
#if VULKAN
    /// Subpass input (descriptor type only available in Vulkan)
    kDescriptorType_InputAttachment = (kDescriptorType_IndirectCommandBuffer << 1),
    kDescriptorType_TexelBuffer = (kDescriptorType_InputAttachment << 1),
    kDescriptorType_RWTexelBuffer = (kDescriptorType_TexelBuffer << 1),
    kDescriptorType_CombinedImageSampler = (kDescriptorType_RW_TexelBuffer << 1),

    /// Khronos extension ray tracing
    kDescriptorType_AccelerationStructure = (kDescriptorType_CombinedImageSampler << 1),
    kDescriptorType_AccelerationStructureBuildInput = (kDescriptorType_AccelerationStructure << 1),
    kDescriptorType_ShaderDeviceAddress = (kDescriptorType_AccelerationStructureBuildInput << 1),
    kDescriptorType_ShaderBindingTable = (kDescriptorType_ShaderDeviceAddress << 1),
#endif
};

enum GfxResourceMemoryUsage
{
    /// No intended memory usage specified.
    kMemoryUsage_Unknown = 0,
    /// Memory will be used on device only, no need to be mapped on host.
    kMemoryUsage_GpuOnly = 1,
    /// Memory will be mapped on host. Could be used for transfer to device.
    kMemoryUsage_CpuOnly = 2,
    /// Memory will be used for frequent (dynamic) updates from host and reads on device.
    kMemoryUsage_CpuToGpu = 3,
    /// Memory will be used for writing on device and readback on host.
    kMemoryUsage_GpuToCpu = 4,
};

enum GfxBufferCreationFlags
{
    /// Default flag (Buffer will use aliased memory, buffer will not be cpu accessible until mapBuffer is called)
    kBufferCreationFlag_None = 0x01,
    /// Buffer will allocate its own memory (COMMITTED resource)
    kBufferCreationFlag_OwnMemoryBit = 0x02,
    /// Buffer will be persistently mapped
    kBufferCreationFlag_PersistentMapBit = 0x04,
    /// Use ESRAM to store this buffer
    kBufferCreationFlag_Esram = 0x08,
    /// Flag to specify not to allocate descriptors for the resource
    kBufferCreationFlag_NoDescriptorViewCreation = 0x10,
#if VULKAN
    kBufferCreationFlag_HostVisible = 0x100,
    kBufferCreationFlag_HostCoherent = 0x200,
#endif
};

enum GfxQueueType
{
    kQueueType_Graphics = 0,
    kQueueType_Transfer, 
    kQueueType_Compute,
    kQueueType_COUNT,
};

enum GfxQueueFlag
{
    kQueueFlag_None = 0x0,
    kQueueFlag_DisableGpuTimeout = 0x01,
    kQueueFlag_InitMicroprofile = 0x02,
};

enum GfxQueuePriority
{
    kQueuePriority_Normal,
    kQueuePriority_High,
    kQueuePriority_GlobalRealtime,
    kQueuePriority_COUNT,
};

enum GfxSwapChainCreationFlags
{
    kSwapChainCreationFlag_None = 0x0,
    kSwapChainCreationFlag_EnableFoveatedRendering = 0x1,
};

enum GfxWaveOpsSupportFlags
{
    kWaveOpsSupportFlag_None = 0x0,
    kWaveOpsSupportFlag_BasicBit = 0x001,
    kWaveOpsSupportFlag_VoteBit = 0x002,
    kWaveOpsSupportFlag_ArithmeticBit = 0x004,
    kWaveOpsSupportFlag_BallotBit = 0x008,
    kWaveOpsSupportFlag_ShuffleBit = 0x010,
    kWaveOpsSupportFlag_ShuffleRelativeBit = 0x020,
    kWaveOpsSupportFlag_ClusteredBit = 0x040,
    kWaveOpsSupportFlag_QuadBit = 0x080,
    kWaveOpsSupportFlag_PartitionedBitNV = 0x100,
    kWaveOpsSupportFlag_All = 0x7FFFFFFF,
};

enum GfxGpuPresetLevel
{
    kGpuPreset_None = 0,
    kGpuPreset_Office,      // means it isn't supported
    kGpuPreset_Low,
    kGpuPreset_Medium,
    kGpuPreset_High,
    kGpuPreset_Ultra,
    kGpuPreset_COUNT,
};

enum GfxShadingRateCaps
{
    kShadingRateCaps_NotSupported = 0x0,
    kShadingRateCaps_PerDraw = 0x1,
    kShadingRateCaps_PerTile = kShadingRateCaps_PerDraw << 1, 
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
    /// Texture creation flags (decides memory allocation strategy, sharing access,...)
	GfxTextureCreationFlags     flags;
	/// Width
	U32                         width;
	/// Height
	U32                         height;
	/// Depth (Should be 1 if not a mType is not TEXTURE_TYPE_3D)
	U32                         depth;
	/// Texture array size (Should be 1 if texture is not a texture array or cubemap)
	U32                         array_size;
	/// Number of mip levels
	U32                         mip_levels;
	/// MSAA
	GfxSampleCount              sample_count;
	/// Internal image format
	TinyImageFormat             format;
	/// What state will the texture get created in
	GfxResourceState            start_state;
	/// Optimized clear value (recommended to use this same value when clearing the rendertarget)
	GfxClearValue               clear_value;
	/// The image quality level. The higher the quality, the lower the performance. The valid range is between zero and the value appropriate for mSampleCount
	U32                         sample_quality;
	/// Descriptor creation
	GfxDescriptorType           descriptors;
	const void*                 native_handle;
	/// Debug name used in gpu profile
	const char*                 name;
	/// GPU indices to share this texture
	U32*                        shared_node_indices;
	/// Number of GPUs to share this texture
	U32                         shared_node_index_count;
	/// GPU which will own this texture
	U32                         node_index;
};

struct GfxSubResourceDataDesc
{
    U64 src_offset;
    U32 mip_level;
    U32 array_layer;
#if DX11 || DX12 || METAL || VULKAN
    U32 row_pitch;
    U32 slice_pitch;
#endif
};

struct GfxReadRange
{
    U64 offset;
    U64 size;
};

struct GfxTextureDesc
{
    /// Optimized clear value (recommended to use this same value when clearing the rendertarget)
	GfxClearValue                   clear_value;
	/// Pointer to native texture handle if the texture does not own underlying resource
	const void*                     native_handle;
	/// Debug name used in gpu profile
	const char*                     name;  
	/// GPU indices to share this texture
	U32*                            shader_node_indices;
#if VULKAN
	VkSamplerYcbcrConversionInfo*   pVkSamplerYcbcrConversionInfo;
#endif
	/// Texture creation flags (decides memory allocation strategy, sharing access,...)
	GfxTextureCreationFlags         flags;
	/// Width
	U32                             width;
	/// Height
	U32                             height;
	/// Depth (Should be 1 if not a mType is not TEXTURE_TYPE_3D)
	U32                             depth;
	/// Texture array size (Should be 1 if texture is not a texture array or cubemap)
	U32                             array_size;
	/// Number of mip levels
	U32                             mip_levels;
	/// Number of multisamples per pixel (currently Textures created with mUsage TEXTURE_USAGE_SAMPLED_IMAGE only support SAMPLE_COUNT_1)
	GfxSampleCount                  sample_count;
	/// The image quality level. The higher the quality, the lower the performance. The valid range is between zero and the value appropriate for mSampleCount
	U32                             sample_quality;
	///  image format
	TinyImageFormat                 format;
	/// What state will the texture get created in
	GfxResourceState                start_state;
	/// Descriptor creation
	GfxDescriptorType               descriptors;
	/// Number of GPUs to share this texture
	U32                             shader_node_index_count;
	/// GPU which will own this texture
	U32                             node_index;
};

struct GfxBufferDesc
{
    /// Size of the buffer (in bytes)
	U64                     size;
	/// Set this to specify a counter buffer for this buffer (applicable to BUFFER_USAGE_STORAGE_SRV, BUFFER_USAGE_STORAGE_UAV)
	GfxBuffer               counter_buffer;
	/// Index of the first element accessible by the SRV/UAV (applicable to BUFFER_USAGE_STORAGE_SRV, BUFFER_USAGE_STORAGE_UAV)
	U64                     first_element;
	/// Number of elements in the buffer (applicable to BUFFER_USAGE_STORAGE_SRV, BUFFER_USAGE_STORAGE_UAV)
	U64                     element_count;
	/// Size of each element (in bytes) in the buffer (applicable to BUFFER_USAGE_STORAGE_SRV, BUFFER_USAGE_STORAGE_UAV)
	U64                     struct_stride;
	/// Debug name used in gpu profile
	const char*             name;
	U32                     shared_node_indices;
	/// Alignment
	U32                     alignment;
	/// Decides which memory heap buffer will use (default, upload, readback)
	GfxResourceMemoryUsage  memory_usage;
	/// Creation flags of the buffer
	GfxBufferCreationFlags  flags;
	/// What type of queue the buffer is owned by
	GfxQueueType            queue_type;
	/// What state will the buffer get created in
	GfxResourceState        start_state;
	/// ICB draw type
	GfxIndirectArgumentType icb_draw_type;
	/// ICB max vertex buffers slots count
	U32                     icb_max_command_count;
	/// Format of the buffer (applicable to typed storage buffers (Buffer<T>)
	TinyImageFormat         format;
	/// Flags specifying the suitable usage of this buffer (Uniform buffer, Vertex Buffer, Index Buffer,...)
	GfxDescriptorType       descriptors;
	/// The index of the GPU in SLI/Cross-Fire that owns this buffer, or the Renderer index in unlinked mode.
	U32                     node_index;
	U32                     shared_node_index_count;
};

struct GfxCommandDesc
{
    GfxCommandPool      pool;
    B32                 secondary;
};

struct GfxCommandPoolDesc
{
    GfxQueue            queue;
    B32                 transient;
};

struct GfxSwapChainDesc
{
    /// Window handle
    OSWindow                    window_handle;
    /// Queues which should be allowed to present
    GfxQueue*                   present_queues;
    /// Number of present queues
    U32                         present_queue_count;
    /// Number of backbuffers in this swapchain
    U32                         image_count;
    /// Width of the swapchain
    U32                         width;
    /// Height of the swapchain
    U32                         height;
    /// Color format of the swapchain
    TinyImageFormat             color_format;
    /// Clear value
    GfxClearValue               clear_color_value;
    /// Swapchain creation flags
    GfxSwapChainCreationFlags   flags;
    /// Set whether swap chain will be presented using vsync
    B32                         enable_vsync;
    /// We can toggle to using FLIP model if app desires.
    B32                         use_flip_swap_effect;
};

struct GfxQueueDesc
{
    GfxQueueType        type;
    GfxQueueFlag        flag;
    GfxQueuePriority    priority;
    U32                 node_index;
};

typedef void(*GfxLogFn)(GfxLogLevel, const char*, const char*);

enum GfxShaderTarget
{
#if DX11
    kShaderTarget_5_0,
#endif
    kShaderTarget_5_1,
    kShaderTarget_6_0,
    kShaderTarget_6_1,
    kShaderTarget_6_2,
    kShaderTarget_6_3,
    kShaderTarget_6_4,
};


struct GfxExtendedSettings
{
    U32             count;
    U32*            settings;
    const char**    names;
};

struct GfxGpuContextDesc
{
#if GFX_USE_MULTIPLE_RENDER_APIS
    union {
#endif
#if DX12
        // D3D_FEATURE_LEVEL   dx_feature_level;
#endif
#if VULKAN
        struct {
            const char** instance_layers;
            const char** instance_extensions;
            const char** device_extensions;
            U32          instance_layer_count;
            U32          instance_extension_count;
            U32          device_extension_count;
            /// flag to specify whether to request all queues from the gpu or just one of each type
            /// Will affect memory usage - around 200MB more used if all queues are requested
            B32          request_all_available_queues;
        } vulkan;
#endif
#if DX11
        struct {
            /// force feature level 10 for compatibility
            B32         use_dx10;
            /// pick the first valid gpu or use GpuConfig
            B32         use_default_gpu;
        } d3d11;
#endif
#if GLES
        // TODO(james): Add support for GLES?
#endif
#if GFX_USE_MULTIPLE_RENDER_APIS
    };
#endif
	/// This results in new validation not possible during API calls on the CPU, by creating patched shaders that have validation added directly to the shader.
	/// However, it can slow things down a lot, especially for applications with numerous PSOs. Time to see the first render frame may take several minutes
    B32                     enable_gpu_based_validation;

    GfxLogFn                log_fn;
    GfxShaderTarget         shader_target;
    GfxGpuMode              gpu_mode;

    GfxExtendedSettings*    extended_settings;
    
    // TODO(james): support multiple gpu context creation
    // GfxContent*          context
    U32                     gpu_index;
};

enum GfxApiType
{
    kGfxApi_PlatformDefault = 0,
#if DX11
    kGfxApi_D3D11,
#endif
#if DX12
    kGfxApi_D3D12,
#endif
#if VULKAN
    kGfxApi_Vulkan,
#endif
#if METAL
    kGfxApi_Metal,
#endif
#if GLES
    kGfxApi_GLES,
#endif
};

struct GfxVendorPreset
{
    char                venderId[kMax_GpuVendorStringLength];
    char                modelId[kMax_GpuVendorStringLength];
    char                revisionId[kMax_GpuVendorStringLength];
    GfxGpuPresetLevel   preset_level;
    char                gpu_name[kMax_GpuVendorStringLength];
    char                gpu_driver_version[kMax_GpuVendorStringLength];
    char                gpu_driver_date[kMax_GpuVendorStringLength];
    U32                 rt_cores_count;
};

struct GfxGpuSettings
{
    U64                     vram;
    U32                     uniform_buffer_alignment;
    U32                     upload_buffer_texture_alignment;
    U32                     upload_buffer_texture_row_alignment;
    U32                     max_vertex_input_bindings;
    U32                     max_root_signature_dwords;
    U32                     wave_lane_count;
    GfxWaveOpsSupportFlags  wave_ops_support_flags;
    GfxVendorPreset         gpu_vendor_preset;
    GfxShadingRate          shading_rates;
    GfxShadingRateCaps      shading_rate_caps;
    U32                     shading_rate_texel_width;
    U32                     shading_rate_texel_height;
    U32                     multidraw_indirect : 1;
    U32                     indirect_root_constant : 1;
    U32                     builtin_draw_id : 1;
    U32                     indirect_cmd_buffer : 1;
    U32                     rovs_supported : 1;
    U32                     tessellation_supported : 1;
    U32                     geometry_shader_supported : 1;
    U32                     gpu_breadcrumbs : 1;
    U32                     hdr_supported : 1;
#if VULKAN
    U32                     sampler_anisotropy_supported : 1;
#endif
#if METAL
    U32                     argument_buffer_max_textures;
    U32                     heaps : 1;
    U32                     placement_heaps : 1;
    U32                     tessellation_indirect_draw_supported : 1;
#endif
};

struct GfxGpuCapBits
{
    bool canShaderReadFrom[TinyImageFormat_Count];
    bool canShaderWriteTo[TinyImageFormat_Count];
    bool canShaderTargetWriteTo[TinyImageFormat_Count];
};

struct GfxApi;
function GfxApi gfx_init_api(GfxApiType type);

struct GfxApi
{
    void*   native_handle;

    // TODO(james): Figure out the memory allocation scheme for the Gfx API
    GFX_API_FUNCTION(GfxGpuContext, initGpu, GfxGpuContextDesc* desc);

    GFX_API_FUNCTION(GfxFence, addFence, GfxGpuContext gc);
    GFX_API_FUNCTION(void, removeFence, GfxGpuContext gc, GfxFence fence);

    GFX_API_FUNCTION(GfxSemaphore, addSemaphore, GfxGpuContext gc);
    GFX_API_FUNCTION(void, removeSemaphore, GfxGpuContext gc, GfxSemaphore semaphore);

    GFX_API_FUNCTION(GfxQueue, addQueue, GfxGpuContext gc, GfxQueueDesc* desc);
    GFX_API_FUNCTION(void, removeQueue, GfxGpuContext gc, GfxQueue queue);

    GFX_API_FUNCTION(GfxSwapChain, addSwapChain, GfxGpuContext gc, GfxSwapChainDesc* desc);
    GFX_API_FUNCTION(void, removeSwapChain, GfxGpuContext gc, GfxSwapChain swapchain);

    // command pool operations
    GFX_API_FUNCTION(GfxCommandPool, addCommandPool, GfxGpuContext gc, GfxCommandPoolDesc* desc);
    GFX_API_FUNCTION(void, removeCommandPool, GfxGpuContext gc, GfxCommandPool commandpool);
    GFX_API_FUNCTION(GfxCommand, addCommand, GfxGpuContext gc, GfxCommandPoolDesc* desc);
    GFX_API_FUNCTION(void, removeCommand, GfxGpuContext gc, GfxCommand cmd);
    GFX_API_FUNCTION(void, addCommands, GfxGpuContext gc, GfxCommandDesc* desc, U32 count, GfxCommand* out_commands);
    GFX_API_FUNCTION(void, removeCommands, GfxGpuContext gc, U32 count, GfxCommand* commands);

    //
    //  All buffer, texture loading handled by the resource system -> GfxResourceLoader
    //
    GFX_API_FUNCTION(GfxBuffer, addBuffer, GfxGpuContext gc, GfxBufferDesc* desc);
    GFX_API_FUNCTION(void, removeBuffer, GfxGpuContext gc, GfxBuffer buffer);
    GFX_API_FUNCTION(void, mapBuffer, GfxGpuContext gc, GfxBuffer buffer, GfxReadRange* range);
    GFX_API_FUNCTION(void, unmapBuffer, GfxGpuContext gc, GfxBuffer buffer);
    GFX_API_FUNCTION(void, cmdUpdateBuffer, GfxCommand cmd, GfxBuffer buffer, U64 dstOffset, GfxBuffer srcBuffer, U64 srcOffset, U64 size);
    GFX_API_FUNCTION(void, cmdUpdateSubresource, GfxCommand cmd, GfxTexture texture, GfxBuffer srcBuffer, GfxSubResourceDataDesc* desc);
    GFX_API_FUNCTION(void, cmdCopySubresource, GfxCommand cmd, GfxBuffer dstBuffer, GfxTexture texture, GfxSubResourceDataDesc* desc);
    GFX_API_FUNCTION(GfxTexture, addTexture, GfxGpuContext gc, GfxTextureDesc* desc);
    GFX_API_FUNCTION(void, removeTexture, GfxGpuContext gc, GfxTexture texture);
    GFX_API_FUNCTION(void, addVirtualTexture, GfxCommand cmd, GfxTextureDesc* desc, GfxTexture textures, void* image_data);
    //

    GFX_API_FUNCTION(GfxRenderTarget, addRenderTarget, GfxGpuContext gc, GfxRenderTargetDesc* desc);
    GFX_API_FUNCTION(void, removeRenderTarget, GfxGpuContext gc, GfxRenderTarget rendertarget);
    GFX_API_FUNCTION(GfxSampler, addSampler, GfxGpuContext gc, GfxSamplerDesc* desc);
    GFX_API_FUNCTION(void, removeSampler, GfxGpuContext gc, GfxSampler sampler);

    // shader operations
    GFX_API_FUNCTION(GfxShader, addShaderBinary, GfxGpuContext gc, GfxShaderDesc* desc);
    GFX_API_FUNCTION(void, removeShader, GfxGpuContext gc, GfxShader shader);

    GFX_API_FUNCTION(GfxRootSignature, addRootSignature, GfxGpuContext gc, GfxRootSignatureDesc* desc);
    GFX_API_FUNCTION(void, removeRootSignature, GfxGpuContext gc, GfxRootSignature rootsignature);

    // pipeline operations
    GFX_API_FUNCTION(GfxPipeline, addPipeline, GfxGpuContext gc, GfxPipelineDesc* desc);
    GFX_API_FUNCTION(void, removePipeline, GfxGpuContext gc, GfxPipeline pipeline);
    GFX_API_FUNCTION(GfxPipelineCache, addPipelineCache, GfxGpuContext gc, GfxPipelineCacheDesc* desc);
    GFX_API_FUNCTION(void, getPipelineCacheData, GfxGpuContext gc, GfxPipelineCache pipelinecache, U64* out_size, void* out_data);
    GFX_API_FUNCTION(void, removePipelineCache, GfxGpuContext gc, GfxPipelineCache pipelinecache);

    // descriptor set operations
    GFX_API_FUNCTION(GfxDescriptorSet, addDescriptorSet, GfxGpuContext gc, GfxDescriptorSetDesc* desc);
    GFX_API_FUNCTION(void, removeDescriptorSet, GfxGpuContext gc, GfxDescriptorSet descriptorset);
    GFX_API_FUNCTION(void, updateDescriptorSet, GfxGpuContext gc, U32 index, GfxDescriptorSet descriptorset, U32 count, GfxDescriptorData* params);

    // command buffer operations
    GFX_API_FUNCTION(void, resetCommandPool, GfxGpuContext gc, GfxCommandPool commandpool);
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
    GFX_API_FUNCTION(void, acquireNextImage, GfxGpuContext gc, GfxSwapChain swapchain, GfxSemaphore signal_semaphore, GfxFence fence, U32* out_image_index);
    GFX_API_FUNCTION(void, queueSubmit, GfxQueue queue, GfxQueueSubmitDesc* desc);
    GFX_API_FUNCTION(void, queuePresent, GfxQueue queue, GfxQueuePresentDesc* desc);
    GFX_API_FUNCTION(void, waitQueueIdle, GfxQueue queue);
    GFX_API_FUNCTION(GfxFenceStatus, getFenceStatus, GfxGpuContext gc, GfxFence fence);
    GFX_API_FUNCTION(void, waitForFences, GfxGpuContext gc, U32 count, GfxFence* fences);
    GFX_API_FUNCTION(void, toggleVSync, GfxGpuContext gc, GfxSwapChain swapchain);

    //Returns the recommended format for the swapchain.
    //If true is passed for the hintHDR parameter, it will return an HDR format IF the platform supports it
    //If false is passed or the platform does not support HDR a non HDR format is returned.
    //If true is passed for the hintSrgb parameter, it will return format that is will do gamma correction automatically
    //If false is passed for the hintSrgb parameter the gamma correction should be done as a postprocess step before submitting image to swapchain
    GFX_API_FUNCTION(TinyImageFormat, getRecommendedSwapchainFormat, bool hintHDR, bool hintSRGB);

    // indirect draw operations
    GFX_API_FUNCTION(GfxCommandSignature, addIndirectCommandSignature, GfxGpuContext gc, GfxCommandSignatureDesc* desc);
    GFX_API_FUNCTION(void, removeIndirectCommandSignature, GfxGpuContext gc, GfxCommandSignature cmdsignature);
    GFX_API_FUNCTION(void, cmdExecuteIndirect, GfxCommand cmd, GfxCommandSignature cmdsignature, U32 max_cmd_count, GfxBuffer indirect_buffer, U64 buffer_offset, GfxBuffer counter_buffer, U64 counter_buffer_offset);

    //
    // GPU Query Interface
    //
    GFX_API_FUNCTION(F64, getTimestampFrequency, GfxQueue queue);
    GFX_API_FUNCTION(GfxQueryPool, addQueryPool, GfxGpuContext gc, GfxQueryPoolDesc* desc);
    GFX_API_FUNCTION(void, removeQueryPool, GfxGpuContext gc, GfxQueryPool querypool);
    GFX_API_FUNCTION(void, cmdResetQueryPool, GfxCommand cmd, GfxQueryPool querypool, U32 start_query, U32 query_count);
    GFX_API_FUNCTION(void, cmdBeginQuery, GfxCommand cmd, GfxQueryPool querypool, GfxQueryDesc* desc);
    GFX_API_FUNCTION(void, cmdEndQuery, GfxCommand cmd, GfxQueryPool querypool, GfxQueryDesc* desc);
    GFX_API_FUNCTION(void, cmdResolveQuery, GfxCommand cmd, GfxQueryPool querypool, GfxBuffer readback_buffer, U32 start_query, U32 query_count);
    //
    // Stats Info Interface
    //
    GFX_API_FUNCTION(void, calculateMemoryStats, MemArena* arena, GfxGpuContext gc, String8List* out_stats);
    GFX_API_FUNCTION(void, calculateMemoryUse, GfxGpuContext gc, U64* out_usedbytes, U64* out_totalallocatedbytes);
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
    GFX_API_FUNCTION(void, setBufferName, GfxGpuContext gc, GfxBuffer buffer, String8 name);
    GFX_API_FUNCTION(void, setTextureName, GfxGpuContext gc, GfxTexture texture, String8 name);
    GFX_API_FUNCTION(void, setRenderTargetName, GfxGpuContext gc, GfxRenderTarget rendertarget, String8 name);
    GFX_API_FUNCTION(void, setPipelineName, GfxGpuContext gc, GfxPipeline pipeline, String8 name);
    //
    GFX_API_FUNCTION(U32, getDescriptorIndexFromName, GfxRootSignature rootsignature, String8 name);
};

#endif
