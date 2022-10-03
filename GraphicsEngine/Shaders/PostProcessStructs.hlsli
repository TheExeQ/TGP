#pragma once

cbuffer FrameBuffer : register(b0)
{
    float4x4 FB_ToView;
    float4x4 FB_ToProjection;
    float3 FB_CamTranslation;
    uint FB_RenderMode;
    float2 FB_Resolution;
    float2 padding;
}

struct PostProcessVertexToPixel
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct PostProcessPixelOutput
{
    float4 Color : SV_TARGET;
};

SamplerState defaultSampler : register(s0);

Texture2D TextureSlot1 : register(t0);
Texture2D TextureSlot2 : register(t1);