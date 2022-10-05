#define MAX_LIGHTS 8u

struct VertexInput
{
    float4 myPosition : POSITION;
    float4 myVxColor : COLOR0;
    float4 myVxColor2 : COLOR1;
    float4 myVxColor3 : COLOR2;
    float4 myVxColor4 : COLOR3;
    float2 myUV : UVS0;
    float2 myUV2 : UVS1;
    float2 myUV3 : UVS2;
    float2 myUV4 : UVS3;
    uint4 myBoneIDs : BONEIDS;
    float4 myBoneWeights : BONEWEIGHTS;
    float3 myTangent : TANGENT;
    float3 myBinormal : BINORMAL;
    float3 myNormal : NORMAL;
    float4x4 World : WORLD;
    uint InstanceId : SV_InstanceID;
};

struct VertexToPixel
{
    float4 myPosition : SV_POSITION;
    float4 myVxPosition : POSITION;
    float4 myVxColor : COLOR0;
    float4 myVxColor2 : COLOR1;
    float4 myVxColor3 : COLOR2;
    float4 myVxColor4 : COLOR3;
    float2 myUV : UVS0;
    float2 myUV2 : UVS1;
    float2 myUV3 : UVS2;
    float2 myUV4 : UVS3;
    float3 myTangent : TANGENT;
    float3 myBinormal : BINORMAL;
    float3 myNormal : NORMAL;
};

struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

struct DeferredVertexInput
{
    unsigned int myIndex : SV_VertexID;
};

struct DeferredVertexToPixel
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct DeferredPixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 FB_ToView;
    float4x4 FB_ToProjection;
    float3 FB_CamTranslation;
    uint FB_RenderMode;
    float2 FB_Resolution;
    float FB_Near;
    float FB_Far;
    float4 FB_FrustumCorners[4];
}

cbuffer Objectbuffer : register(b1)
{
    float4x4 OB_ToWorld;
    float4x4 OB_BoneData[128];
    bool OB_HasBones = false;
    bool OB_Instanced = false;
    float2 obpadding7;
}

cbuffer MaterialBuffer : register(b2)
{
    float3 MB_Albedo;
    float padding1;
}

struct LightData
{
    float4x4 LightView;
    float4x4 LightProj;
    
    float3 Color;
    float Intensity;
    float3 Direction;
    float Range;
    float3 Position;
    float Attenuation;

    float SpotInnerRadius;
    float SpotOuterRadius;
    int LightType;
    bool CastShadows;
    
    float Near;
    float Far;
    
    float2 Padding;
};

cbuffer LightBuffer : register(b3)
{
    LightData LB_DirectionalLight;
    LightData LB_Lights[MAX_LIGHTS];
    uint LB_NumLights;
    float3 LB_Padding;
}

struct GBufferOutput
{
    float4 Albedo : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 Material : SV_TARGET2;
    float4 VertexNormal : SV_TARGET3;
    float4 WorldPosition : SV_TARGET4;
    float4 AmbientOcclusion : SV_TARGET5;
};

SamplerState defaultSampler : register(s0);
SamplerState wrapSampler : register(s1);
SamplerState pointClampSampler : register(s2);
SamplerState pointWrapSampler : register(s3);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D materialTexture : register(t2);
Texture2D vertexNormalTexture : register(t3);
Texture2D worldPositionTexture : register(t4);
Texture2D ambientOcclusionTexture : register(t5);
Texture2D dirLightShadowMap : register(t6);
Texture2D spotLightShadowMap : register(t7);
Texture2D SSAOTexture : register(t8);
TextureCube environmentTexture : register(t14);