#include "LightAssetHandler.h"
#include "Math/Matrix.hpp"
#include "TextureAssetHandler.h"
#include "Core/GraphicsEngine.h"

Ref<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3<float> aColor, float aIntensity, 
	Vector3<float> aPosition, Vector3<float> aRotation)
{
	myLights.push_back(CreateRef<DirectionalLight>());
	myDirectionalLight = std::dynamic_pointer_cast<DirectionalLight>(myLights.back());
	myDirectionalLight->Init(aColor, aIntensity);
	myDirectionalLight->ourLightBuffer.Position = aPosition;

	auto dir = Matrix4::Direction(aRotation);

	myDirectionalLight->ourLightBuffer.Direction = Vector3f(dir.x, dir.y, dir.z);
	myDirectionalLight->ourLightBuffer.LightType = 0;

	constexpr float nearPlane = 0.001f;
	constexpr float farPlane = 25000.f;

	POINT res = { 2048, 2048 };

	myDirectionalLight->ourLightBuffer.Near = nearPlane;
	myDirectionalLight->ourLightBuffer.Far = farPlane;

	Matrix4x4<float> lightProj;

	lightProj(1, 1) = 2.f / static_cast<float>(res.x);
	lightProj(2, 2) = 2.f / static_cast<float>(res.y);
	lightProj(3, 3) = 1.f / (farPlane - nearPlane);
	lightProj(4, 3) = nearPlane / (nearPlane - farPlane);
	lightProj(4, 4) = 1.f;

	Matrix4x4<float> lightView;
	lightView = Matrix4x4<float>::Rotate(lightView, aRotation);
	lightView = Matrix4x4<float>::Translate(lightView, myDirectionalLight->GetLightBufferData().Position);

	myDirectionalLight->ourLightBuffer.LightProj = lightProj;
	myDirectionalLight->ourLightBuffer.LightView = Matrix4x4<float>::GetFastInverse(lightView);

	myDirectionalLight->myShadowMaps[0] = TextureAssetHandler::CreateDepthStencil("directShadow", res.x, res.y);

	myDirectionalLight->ourLightBuffer.CastShadows = true;

	return myDirectionalLight;
}

Ref<PointLight> LightAssetHandler::CreatePointLight(Vector3<float> aColor, float aIntensity, float aRange, float fov, float aspectRatio)
{
	myLights.push_back(CreateRef<PointLight>());
	Ref<PointLight> result = std::dynamic_pointer_cast<PointLight>(myLights.back());

	result->Init(aColor, aIntensity);
	result->SetRange(aRange);
	result->ourLightBuffer.LightType = 1;

	constexpr float nearPlane = 0.001f;
	constexpr float farPlane = 25000.f;

	POINT res = { 2048, 2048 };

	const float hFOV = 90.f * (3.14f / 180);
	const float vFOV = 2 * std::atan(std::tan(hFOV * 0.5f) * aspectRatio);

	const float myXScale = 1 / std::tanf(hFOV * 0.5f);
	const float myYScale = 1 / std::tanf(vFOV * 0.5f);
	const float Q = farPlane / (farPlane - nearPlane);

	Matrix4x4<float> lightProj;

	lightProj(1, 1) = myXScale;
	lightProj(2, 2) = myYScale;
	lightProj(3, 3) = Q;
	lightProj(3, 4) = 1.f / Q;
	lightProj(4, 3) = -Q * nearPlane;
	lightProj(4, 4) = 0.f;

	result->myShadowMaps[0] = TextureAssetHandler::CreateDepthStencil("pointShadow1", res.x, res.y);
	result->myShadowMaps[1] = TextureAssetHandler::CreateDepthStencil("pointShadow2", res.x, res.y);
	result->myShadowMaps[2] = TextureAssetHandler::CreateDepthStencil("pointShadow3", res.x, res.y);
	result->myShadowMaps[3] = TextureAssetHandler::CreateDepthStencil("pointShadow4", res.x, res.y);
	result->myShadowMaps[4] = TextureAssetHandler::CreateDepthStencil("pointShadow5", res.x, res.y);
	result->myShadowMaps[5] = TextureAssetHandler::CreateDepthStencil("pointShadow6", res.x, res.y);

	result->ourLightBuffer.LightProj = lightProj;
	result->ourLightBuffer.CastShadows = true;

	return result;
}

Ref<SpotLight> LightAssetHandler::CreateSpotLight(Vector3<float> aColor, float aIntensity, float aRange, float aInner, float aOuter, 
	float fov, float aspectRatio)
{
	myLights.push_back(CreateRef<SpotLight>());
	Ref<SpotLight> result = std::dynamic_pointer_cast<SpotLight>(myLights.back());

	result->Init(aColor, aIntensity);
	result->SetRange(aRange);
	result->SetInnerCone(aInner);
	result->SetOuterCone(aOuter);
	result->ourLightBuffer.LightType = 2;

	constexpr float nearPlane = 1.0f;
	constexpr float farPlane = 10000.f;

	POINT res = { 2048, 2048 };

	const float hFOV = 90.f * (3.14f / 180);
	const float vFOV = 2 * std::atan(std::tan(hFOV * 0.5f) * aspectRatio);

	const float myXScale = 1 / std::tanf(hFOV * 0.5f);
	const float myYScale = 1 / std::tanf(vFOV * 0.5f);
	const float Q = farPlane / (farPlane - nearPlane);

	Matrix4x4<float> lightProj;

	lightProj(1, 1) = myXScale;
	lightProj(2, 2) = myYScale;
	lightProj(3, 3) = Q;
	lightProj(3, 4) = 1.f / Q;
	lightProj(4, 3) = -Q * nearPlane;
	lightProj(4, 4) = 0.f;

	//lightProj(1, 1) = 2.f / static_cast<float>(res.x);
	//lightProj(2, 2) = 2.f / static_cast<float>(res.y);
	//lightProj(3, 3) = 1.f / (farPlane - nearPlane);
	//lightProj(4, 3) = nearPlane / (nearPlane - farPlane);
	//lightProj(4, 4) = 1.f;


	result->myShadowMaps[0] = TextureAssetHandler::CreateDepthStencil("spotShadow", res.x, res.y);

	result->ourLightBuffer.LightProj = lightProj;
	result->ourLightBuffer.CastShadows = true;

	return result;
}

Ref<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::string& aTextureName)
{
	myEnvironmentLight = CreateRef<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);

	return myEnvironmentLight;
}
