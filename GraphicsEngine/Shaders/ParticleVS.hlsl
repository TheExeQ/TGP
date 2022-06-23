#include <ShaderStructs.hlsli>
#include <ParticleStructs.hlsli>

ParticleVertexData main(ParticleVertexData input)
{
    ParticleVertexData result;
    
    const float4 particleLocalPosition = input.myPosition;
    const float4 particleWorldPosition = mul(OB_ToWorld, particleLocalPosition);
    
    result.myPosition = particleWorldPosition;
    result.myColor = input.myColor,
    result.myLifetime = input.myLifetime;
    result.myScale = input.myScale;
    result.myVelocity = input.myVelocity;
    
	return result;
}