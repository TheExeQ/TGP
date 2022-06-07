#include "ShaderStructs.hlsli"

SamplerState defaultSampler : register(s0);
Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;

    const float3x3 TBN = float3x3
    (
        normalize(input.myTangent),
        normalize(input.myBinormal),
        normalize(input.myNormal)
    );
    
    float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agr;
    normalMap.z = 0;
    
    normalMap = 2.0f * normalMap - 1;
    
    normalMap.z = sqrt(1 - saturate(normalMap.x + normalMap.x + normalMap.y + normalMap.y));
    
    normalMap = normalize(normalMap);
    const float pixelNormal = normalize(mul(normalMap, TBN));
    
    float3 debugNormal = pixelNormal;
    const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
    if (signedLength < 0)
    {
        debugNormal = float3(1 - abs(debugNormal));
    }
    
    //result.myColor.rgb = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
    //result.myColor.a = 1.0f;
    
    result.myColor.rgb = debugNormal;
    result.myColor.a = 1.0f;

    return result;
}