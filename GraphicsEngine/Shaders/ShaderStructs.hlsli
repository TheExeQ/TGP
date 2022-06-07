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

cbuffer FrameBuffer : register(b0)
{
    float4x4 FB_ToView;
    float4x4 FB_ToProjection;
}

cbuffer Objectbuffer : register(b1)
{
    float4x4 OB_ToWorld;
    //float3 OB_Padding = float3(0,0,0);
    float4x4 OB_BoneData[128];
    bool OB_HasBones = false;
}

cbuffer MaterialBuffer : register(b2)
{
    float3 MB_Albedo;
    float padding;
}

cbuffer LightBuffer : register(b3)
{
    float3 LB_Color;
    float LB_Intensity;
    float3 LB_Direction;
    float LB_Padding;
}

SamplerState defaultSampler : register(s0);
Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D environmentTexture : register(t10);