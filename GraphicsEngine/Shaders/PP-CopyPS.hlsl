#include "PostProcessStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput returnValue;
    float4 color = TextureSlot1.Sample(defaultSampler,input.UV);
    if (color.a < 0.05f)
    {
        discard;
        returnValue.Color = 0;
        return returnValue;
    }
    
    returnValue.Color.rgb = TextureSlot1.Sample(defaultSampler, input.UV).rgb;
    returnValue.Color.a = 1.0f;
	
    return returnValue;

}