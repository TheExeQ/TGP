#include <ShaderStructs.hlsli>
#include <ParticleStructs.hlsli>

ParticlePixelOutput main(ParticleGeometryToPixel input)
{
    ParticlePixelOutput result;
    float4 textureColor = albedoTexture.Sample(defaultSampler, input.myUV);
    
    if (textureColor.a <= 0.05f)
    {
        discard;
    }
    
    result.myColor.rgba = textureColor.rgba * input.myColor.rgba;
    return result;
}