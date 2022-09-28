#pragma once

struct PostProcessVertexToPixel
{
    float2 UV : UVS0;
};

struct PostProcessPixelOutput
{
    float4 Color : SV_TARGET;
};