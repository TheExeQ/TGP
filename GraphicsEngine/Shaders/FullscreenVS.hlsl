#include "ShaderStructs.hlsli"

DeferredVertexToPixel main(DeferredVertexInput input)
{
    DeferredVertexToPixel result;

    const float4 pos[3] = {
        float4(-1.0f, -1.0f, 0.0f, 1.0f),
        float4(-1.0f, 3.0f, 0.0f, 1.0f),
        float4(3.0f, -1.0f, 0.0f, 1.0f),
    };
    const float2 uv[3] = {
        float2(0.0f, 1.0f),
        float2(0.0f, -1.0f),
        float2(2.0f, 1.0f)
    };

    result.Position = pos[input.myIndex];
    result.UV = uv[input.myIndex];
    return result;
}
