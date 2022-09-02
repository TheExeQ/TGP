#include "ShaderStructs.hlsli"
#include "Common.hlsli"
#include "PBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;

    const float3x3 TBN = float3x3
    (
        normalize(input.myTangent),
        normalize(input.myBinormal),
        normalize(input.myNormal)
    );
    
    const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
    const float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.myUV);
    
    const float ambientOcclusion = normalMap.b;
    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;
    
    float3 pixelNormal = normalMap * 0.75f;
    pixelNormal.z = 0;
    pixelNormal = 2.0f * pixelNormal - 1;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x + pixelNormal.x + pixelNormal.y + pixelNormal.y));
    pixelNormal = normalize(pixelNormal);
    pixelNormal = normalize(mul(pixelNormal, TBN));
    
    const float3 toEye = normalize(FB_CamTranslation.xyz - input.myVxPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04f, albedo, metalness);
    const float3 diffuseColor = lerp((float3) 0.00f, albedo, 1 - metalness);
    
    const float3 ambientLighting = EvaluateAmbience(
    environmentTexture,
    pixelNormal,
    input.myNormal,
    toEye,
    roughness,
    ambientOcclusion,
    diffuseColor,
    specularColor);
    
    const float3 directLighting = EvaluateDirectionalLight(
    diffuseColor, 
    specularColor,
    pixelNormal,
    roughness,
    LB_Color,
    LB_Intensity,
    -LB_Direction,
    toEye);
    
    result.myColor.rgb = LinearToGamma(directLighting + ambientLighting);
    result.myColor.a = 1.0f;

    return result;
}