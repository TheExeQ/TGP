#pragma once
#include "ShaderStructs.hlsli"
#include "Common.hlsli"
#include "PBRFunctions.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
    DeferredPixelOutput result;

    const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);
    if (albedo.a == 0)
    {
        result.myColor = 0;
        discard;
    }

    const float3 pixelNormal = normalTexture.Sample(defaultSampler, input.myUV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.myUV);
    const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.myUV).rgb;
    const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.myUV);
    const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.myUV).r;

    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;

    const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04f, albedo.xyz, metalness);
    const float3 diffuseColor = lerp((float3) 0.00f, albedo.xyz, 1 - metalness);

    const float3 ambientLighting = EvaluateAmbience(
        environmentTexture,
        pixelNormal,
        vertexNormal,
        toEye,
        roughness,
        ambientOcclusion,
        diffuseColor,
        specularColor);
    
    float3 directLighting = EvaluateDirectionalLight(
        diffuseColor,
        specularColor,
        pixelNormal,
        roughness,
        LB_DirectionalLight.Color,
        LB_DirectionalLight.Intensity,
        -LB_DirectionalLight.Direction,
        toEye);
    
    float3 pointLight = 0;
    float3 spotLight = 0;
    
    for (unsigned int l = 0; l < LB_NumLights; l++)
    {
        const LightData light = LB_Lights[l];
        
        switch (light.LightType)
        {
            case 0:
                // Directional
                break;
            
            case 1:
                pointLight += EvaluatePointLight(
            diffuseColor,
            specularColor,
            pixelNormal,
            material.g,
            light.Color,
            light.Intensity,
            light.Range,
            light.Position,
            toEye,
            worldPosition.xyz
            );
                break;
            
            case 2:
                pointLight += EvaluateSpotLight(
            diffuseColor,
            specularColor,
            pixelNormal,
            material.g,
            light.Color,
            light.Intensity,
            light.Range,
            light.Position,
            light.Direction,
            light.SpotOuterRadius,
            light.SpotInnerRadius,
            toEye,
            worldPosition.xyz
            );
                break;
                
        }

    }
    
    result.myColor.rgb = LinearToGamma(directLighting + ambientLighting + pointLight + spotLight);
    result.myColor.a = 1.0f;

    return result;
}
