#include "ShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;

    const float3x3 TBN = float3x3
    (
        normalize(input.myTangent),
        normalize(input.myBinormal),
        normalize(input.myNormal)
    );
    
    const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
    const float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.myUV);
    
    const float ambientOcclusion = normalMap.b;
    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;
    
    float3 pixelNormal = normalMap * 0.75f;
    pixelNormal.z = 0;
    pixelNormal = 2.0f * pixelNormal - 1;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x + pixelNormal.x + pixelNormal.y + pixelNormal.y));
    pixelNormal = normalize(pixelNormal);
    pixelNormal = normalize(mul(pixelNormal, TBN));
    
    const float3 toEye = normalize(FB_CamTranslation.xyz - input.myVxPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04f, albedo, metalness);
    const float3 diffuseColor = lerp((float3) 0.00f, albedo, 1 - metalness);
    
    const float3 ambientLighting = EvaluateAmbience(
    environmentTexture,
    pixelNormal,
    input.myNormal,
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
    
	if (LB_DirectionalLight.CastShadows)
	{
		const float4 worldToLightView = mul(LB_DirectionalLight.LightView, input.myVxPosition);
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
            input.myVxPosition.xyz
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
            input.myVxPosition.xyz
            );
                break;
                
        }

    }
    
    result.myColor.rgb = LinearToGamma(directLighting + ambientLighting + pointLight + spotLight);
    result.myColor.a = 1.0f;

    return result;
}