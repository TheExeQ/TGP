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
    bool OB_HasBones;
    float3 OB_Padding;
    float4x4 OB_BoneData[128];
}

cbuffer MaterialBuffer : register(b2)
{
    float3 MB_Albedo;
    float padding;
}