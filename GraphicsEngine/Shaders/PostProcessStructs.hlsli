
cbuffer FrameBuffer : register(b0)
{
    float4x4 FB_ToView;
    float4x4 FB_ToProjection;
    float3 FB_CamTranslation;
    uint FB_RenderMode;
    float2 FB_Resolution;
    float FB_Near;
    float FB_Far;
    float4 FB_FrustumCorners[4];
}

struct PostProcessVertexToPixel
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct PostProcessPixelOutput
{
    float4 Color : SV_TARGET;
};

SamplerState defaultSampler : register(s0);

Texture2D TextureSlot1 : register(t0);
Texture2D TextureSlot2 : register(t1);
Texture2D TextureSlot3 : register(t2);
Texture2D TextureSlot4 : register(t3);
Texture2D TextureSlot5 : register(t4);
Texture2D TextureSlot6 : register(t5);
Texture2D TextureSlot7 : register(t6);
Texture2D TextureSlot8 : register(t7);
Texture2D TextureSlot9 : register(t8);
Texture2D TextureSlot10 : register(t9);
Texture2D TextureSlot11 : register(t10);
Texture2D TextureSlot12 : register(t11);

float4 GetViewPosition(float2 uv)
{
    const float4 worldPosition = float4(TextureSlot5.Sample(defaultSampler, uv).rgb, 1);
    const float4 viewPosition = mul(FB_ToView, worldPosition);
    return viewPosition;
}

float4 GetViewNormal(float2 uv)
{
    const float4 worldNormal = float4(TextureSlot2.Sample(defaultSampler, uv).rgb, 0);
    const float4 viewNormal = mul(FB_ToView, worldNormal);
    return viewNormal;
}

float2 GetRandom(float2 uv, float2 uvScale)
{
    const float3 random = 2.f * TextureSlot9.Sample(defaultSampler, uv).rgb - 1.f;
    return random.xy;
}

float SSAO(float2 screenUV, float2 uv, float3 pxViewPos, float3 pxViewNorm,
    float scale, float bias, float intensity)
{
    const float3 diff = GetViewPosition(screenUV + uv).xyz - pxViewPos;
    const float3 v = normalize(diff);
    const float d = length(diff) * scale;
    const float occ = max(0.f, dot(pxViewNorm, v) - bias) * 1.f / (1.f + d) * intensity;

    return occ;
}