#include "ShaderStructs.hlsli"

SamplerState defaultSampler : register(s0);
Texture2D albedoTexture : register(t0);

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;

    //const float average = (input.myVxColor.r + input.myVxColor.g + input.myVxColor.b) / 3.0f;
    //result.myColor = average * float4(MB_Albedo, 1.f);
    
    // For UV Testing
    //result.myColor = float4(input.myUV.r, input.myUV.g, 0, 1);
    
    result.myColor.rgb = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
    result.myColor.a = 1.0f;

    return result;
}