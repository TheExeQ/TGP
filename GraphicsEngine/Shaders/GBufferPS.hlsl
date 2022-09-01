#pragma once
#include "ShaderStructs.hlsli"

GBufferOutput main(VertexToPixel input)
{
	GBufferOutput result;

	const float3x3 TBN = float3x3
		(
			normalize(input.myTangent),
			normalize(input.myBinormal),
			normalize(input.myNormal)
		);

	const float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
	const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
	const float4 material = materialTexture.Sample(defaultSampler, input.myUV);

	const float ambientOcclusion = normalMap.b;

	float3 pixelNormal = normalMap;

	result.Albedo = float4(albedo, 1.f);
	result.Normal = float4(pixelNormal, 1.f);
	result.Material = material;
	result.VertexNormal = float4(normalize(input.myNormal), 1.f);
	result.WorldPosition = input.myVxPosition;
	result.AmbientOcclusion = ambientOcclusion;

	return result;
}