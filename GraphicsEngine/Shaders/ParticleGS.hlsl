#include <ShaderStructs.hlsli>
#include <ParticleStructs.hlsli>

[maxvertexcount(4)]
void main(
	point ParticleVertexData input[1], 
	inout TriangleStream<ParticleGeometryToPixel> output)
{
    const float2 offsets[4] =
    {
        { -1.0f, 1.0f },
        { 1.0f, 1.0f },
        { -1.0f, -1.0f },
        { 1.0f, -1.0f },
    };
    
    const float2 uvs[4] =
    {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
    };
    
    const ParticleVertexData inputParticle = input[0];
    for (unsigned int index = 0; index < 4; index++)
    {
        ParticleGeometryToPixel result;
        result.myPosition = mul(FB_ToView, inputParticle.myPosition);
        result.myPosition.xy += offsets[index] * inputParticle.myScale.xy;
        result.myPosition = mul(FB_ToProjection, result.myPosition);
        result.myColor = inputParticle.myColor;
        result.myLifetime = inputParticle.myLifetime;
        result.myUV = uvs[index];
        result.myVelocity = inputParticle.myVelocity;
        output.Append(result);
    }

}