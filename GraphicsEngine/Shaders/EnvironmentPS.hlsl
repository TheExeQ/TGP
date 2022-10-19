#include "ShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
    DeferredPixelOutput result;

    const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);
    if (albedo.a == 0)
    {
        result.myColor = 0;
        discard;
    }

    const float3 pixelNormal = normalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.UV);
    const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.UV);
    const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.UV).r;

    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;

    const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04f, albedo.xyz, metalness);
    const float3 diffuseColor = lerp((float3) 0.00f, albedo.xyz, 1 - metalness);

    const float ssaoValue = SSAOTexture.Sample(defaultSampler, input.UV).r;
    
    float3 ambientLighting = EvaluateAmbience(
		environmentTexture,
		pixelNormal,
		vertexNormal,
		toEye,
		roughness,
		ambientOcclusion,
		diffuseColor,
		specularColor);
    
    ambientLighting *= ssaoValue;

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

    if (LB_DirectionalLight.CastShadows)
    {
        const float4 worldToLightView = mul(LB_DirectionalLight.LightView, worldPosition);
        const float4 lightViewToLightProj = mul(LB_DirectionalLight.LightProj, worldToLightView);

        float2 projectedTexCoord;
        projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.f + 0.5f;
        projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.f + 0.5f;

        if (saturate(projectedTexCoord.x) == projectedTexCoord.x &&
			saturate(projectedTexCoord.y) == projectedTexCoord.y)
        {
            const float shadowBias = 0.0005f;
            const float shadow = 0.f;
            const float viewDepth = (lightViewToLightProj.z / lightViewToLightProj.w) - shadowBias;
            const float lightDepth = dirLightShadowMap.Sample(pointClampSampler, projectedTexCoord).r;

            if (lightDepth < viewDepth)
            {
				directLighting *= shadow;
            }
        }
    }

    for (unsigned int l = 0; l < LB_NumLights; l++)
    {
        const LightData light = LB_Lights[l];

        switch (light.LightType)
        {
            case 0:
			// Directional
                break;

            case 1:
			//if (light.CastShadows)
			//{
			//	const float4 worldToLightView = mul(light.LightView, worldPosition);
			//	const float4 lightViewToLightProj = mul(light.LightProj, worldToLightView);

			//	float2 projectedTexCoord;
			//	projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.f + 0.5f;
			//	projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.f + 0.5f;

			//	if (saturate(projectedTexCoord.x) == projectedTexCoord.x &&
			//		saturate(projectedTexCoord.y) == projectedTexCoord.y)
			//	{
			//		const float shadowBias = 0.0005f;
			//		const float shadow = 0.f;
			//		const float viewDepth = (lightViewToLightProj.z / lightViewToLightProj.w) - shadowBias;
			//		const float lightDepth = dirLightShadowMap.Sample(pointClampSampler, projectedTexCoord).r;

			//		if (lightDepth < viewDepth)
			//		{
			//			directLighting *= shadow;
			//			break;
			//		}
			//	}
			//}
			
                pointLight += EvaluatePointLight(
				diffuseColor,
				specularColor,
				pixelNormal,
				roughness,
				light.Color,
				light.Intensity,
				light.Range,
				light.Position,
				toEye,
				worldPosition.xyz
			);
                break;

            case 2:
                if (light.CastShadows)
                {
                    const float4 worldToLightView = mul(light.LightView, worldPosition);
                    const float4 lightViewToLightProj = mul(light.LightProj, worldToLightView);

                    float2 projectedTexCoord;
                    projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.f + 0.5f;
                    projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.f + 0.5f;

                    if (saturate(projectedTexCoord.x) == projectedTexCoord.x &&
					saturate(projectedTexCoord.y) == projectedTexCoord.y)
                    {
                        const float shadowBias = 0.000005f;
                        const float viewDepth = (lightViewToLightProj.z / lightViewToLightProj.w) - shadowBias;
                        const float lightDepth = spotLightShadowMap.Sample(pointClampSampler, projectedTexCoord).r;

                        if (lightDepth < viewDepth)
                        {
                            break;
                        }
                    }
                }
			
				spotLight += EvaluateSpotLight(
					diffuseColor,
					specularColor,
					pixelNormal,
					roughness,
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

    result.myColor.rgb = directLighting + ambientLighting + pointLight + spotLight;
    result.myColor.a = 1.0f;

    return result;
}
