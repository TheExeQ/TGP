#pragma once

struct ParticleVertexData
{
    float4 myPosition : POSITION;
    float4 myColor : COLOR;
    float3 myVelocity : VELOCITY;
    float3 myScale : SCALE;
    float myLifetime : LIFETIME;
};

struct ParticleGeometryToPixel
{
    float4 myPosition : SV_POSITION;
    float4 myColor : COLOR;
    float3 myVelocity : VELOCITY;
    float myLifetime : LIFETIME;
    float2 myUV : TEXCOORD;
};

struct ParticlePixelOutput
{
    float4 myColor : SV_TARGET;
};