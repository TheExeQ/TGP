#include "PostProcessStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput result;
	
    const float Pi2 = 6.28318530718;
    
    const float Directions = 16.0f;
    const float Quality = 4.0f;
    const float Radius = 2.0f;
    
    const float2 rad = Radius /*/ FB_Resolution*/;
    
    float4 color = TextureSlot1.Sample(defaultSampler, input.UV);
    
    if (color.a < 0.05f)
    {
        discard;
        result.Color = 0;
        return result;
    }
    
    for (float d = 0.0f; d < Pi2; d += Pi2 / Directions)
    {
        for (float i = 1.0f / Quality; i <= 1.0f; i += 1.0f / Quality)
        {
            color += TextureSlot1.Sample(defaultSampler, input.UV + float2(cos(d), sin(d)) * rad * i);
        }
    }

    color /= Quality * Directions - 15.0f;
    result.Color.rgb = color.rgb;
    result.Color.a = 1.0f;
    
    return result;
}