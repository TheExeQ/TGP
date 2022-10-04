#include "PostProcessStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
	PostProcessPixelOutput result;

	const float intensity = 0.35f;
	const float scale = 0.05f;
	const float bias = 0.05f;
	const float radius = 0.002f;
	const float offset = 0.707f;

	int noiseWidth = 0;
	int noiseHeight = 0;
	int numMips = 0;

	TextureSlot8.GetDimensions(0, noiseWidth, noiseHeight, numMips);
	const float2 randomUVScale = FB_Resolution / float2(noiseWidth, noiseHeight);
	const float2 random = GetRandom(input.UV, randomUVScale);

	const float4 pxPos = GetViewPosition(input.UV);
	const float4 pxNrm = GetViewNormal(input.UV);

	const float2 frustrumCorners[4] = { float2(1, 0), float2(-1, 0), float2(0, 1), float2(0, -1) };

	float occlusion = 0.0f;

	for (uint i = 0; i < 4; i++)
	{
		const uint idx = i % 4;
		const float2 coord1 = reflect(frustrumCorners[idx], random) * radius;
		const float2 coord2 = float2(coord1.x * offset - coord1.y * offset, coord1.x * offset + coord1.y * offset);

		occlusion += SSAO(input.UV, coord1 * 0.25, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.UV, coord2 * 0.5, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.UV, coord1 * 0.75, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.UV, coord2, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
	}

	result.Color.rgb = 1 - occlusion;
	result.Color.a = 1;

	//TEST VIEW NORMAL DEBUG
    //result.Color.rgb = (GetViewNormal(input.UV) + 1.0f) / 2.0f;
    //result.Color.a = 1;

	//TEST VIEW POSITION DEBUG
    //result.Color.rgb = (GetViewPosition(input.UV) + 1.0f) / 2.0f;
    //result.Color.a = 1;

	return result;
}