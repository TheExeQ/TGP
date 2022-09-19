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
    float4 myPosition : SV_POSITION;
    float2 myUV : TEXCOORD;
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
}

cbuffer Objectbuffer : register(b1)
{
    float4x4 OB_ToWorld;
    float4x4 OB_BoneData[128];
    bool OB_HasBones = false;
}

cbuffer MaterialBuffer : register(b2)
{
    float3 MB_Albedo;
    float padding;
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
SamplerState pointClampSampler : register(s1);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D materialTexture : register(t2);
Texture2D vertexNormalTexture : register(t3);
Texture2D worldPositionTexture : register(t4);
Texture2D ambientOcclusionTexture : register(t5);
Texture2D dirLightShadowMap : register(t6);
Texture2D spotLightShadowMap : register(t7);
Texture2D pointLightShadowMap1 : register(t8);
Texture2D pointLightShadowMap2 : register(t9);
Texture2D pointLightShadowMap3 : register(t10);
Texture2D pointLightShadowMap4 : register(t11);
Texture2D pointLightShadowMap5 : register(t12);
Texture2D pointLightShadowMap6 : register(t13);
TextureCube environmentTexture : register(t14);