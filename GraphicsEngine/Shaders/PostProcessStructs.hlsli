#pragma once

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