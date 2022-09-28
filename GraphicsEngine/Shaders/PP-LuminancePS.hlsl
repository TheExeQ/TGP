#pragma once
#include "PostProcessStructs.hlsli"
#include "ShaderStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput result;
    
    const float4 color = albedoTexture.Sample(defaultSampler, input.UV);
    
    // Discard transparent
    if (color.a < 0.05f)
    {
        discard;
    }
    const float3 resource = color.rgb;
    
    // Gain no cutoff
    {
        float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
        result.Color.rgb = resource * pow(luminance, 5);
    }
    
    // Flat cutoff
    {
        float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
        float cutOff = 0.8f;
        if (luminance >= cutOff)
        {
            result.Color.rgb = resource;
        }
        else
        {
            result.Color.rgb = 0.f;
        }
    }
    
    // Gain cutoff
    {
        float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
        float cutOff = 0.5f;
        luminance = saturate(luminance - cutOff);
        result.Color.rgb = resource * luminance * (1.f / cutOff);
    }
    
    // Flat cutoff with smooth edges
    {
        float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
        float cutOff = 0.8f;
        if (luminance >= cutOff)
        {
            result.Color.rgb = resource;
        }
        else if (luminance >= cutOff * 0.5f)
        {
            float fade = luminance / cutOff;
            fade = pow(fade, 5);
            result.Color.rgb = resource * fade;
        }
        else
        {
            result.Color.rgb = 0.f;
        }
    }
    
    return result;
}
