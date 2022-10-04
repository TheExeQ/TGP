#include "PostProcessStructs.hlsli"
#include "Common.hlsli"
#include "Tonemaps.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput result;
	
    const float3 color = TextureSlot1.Sample(defaultSampler, input.UV).rgb;
    const uint mode = 2;
    
    switch (mode)
    {
        default:
            result.Color.rgb = LinearToGamma(color);
            break;
        case 1:
            result.Color.rgb = LinearToGamma(Tonemap_Reinhard2(color));
            break;
        case 2: 
            result.Color.rgb = Tonemap_UnrealEngine(color);
            break;
        case 3:
            result.Color.rgb = LinearToGamma(Tonemap_ACES(color));
            break;
        case 4:
            result.Color.rgb = LinearToGamma(Tonemap_Lottes(color));
            break;
    }
    
    result.Color.a = 1;
	
    return result;
}