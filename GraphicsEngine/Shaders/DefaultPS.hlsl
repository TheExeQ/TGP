#include "ShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;

    result.myColor = input.myVxColor;

    return result;
}