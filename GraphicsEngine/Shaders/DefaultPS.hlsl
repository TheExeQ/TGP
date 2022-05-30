#include "ShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;

    const float average = (input.myVxColor.r + input.myVxColor.g + input.myVxColor.b) / 3.0f;
    
    result.myColor = average * float4(MB_Albedo, 1.f);

    return result;
}