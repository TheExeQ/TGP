#include "LightAssetHandler.h"
#include "Math/Matrix.hpp"
#include "TextureAssetHandler.h"
#include "Core/GraphicsEngine.h"

Ref<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3<float> aColor, float aIntensity, 
	Vector3<float> aPosition, Vector3<float> aRotation, Vector3<float> aDirection)
{
	myLights.push_back(CreateRef<DirectionalLight>());
	myDirectionalLight = std::dynamic_pointer_cast<DirectionalLight>(myLights.back());
	myDirectionalLight->Init(aColor, aIntensity);
	myDirectionalLight->ourLightBuffer.Position = aPosition;
	myDirectionalLight->ourLightBuffer.Direction = aDirection;
	myDirectionalLight->ourLightBuffer.LightType = 0;

	constexpr float nearPlane = 1.f;
	constexpr float farPlane = 25000.f;

	POINT res = { 0, 0 };

	RECT clientRect = { 0, 0, 0, 0 };
	GetClientRect(GraphicsEngine::Get().GetWindowHandle(), &clientRect);

	res.x = clientRect.right - clientRect.left;
	res.y = clientRect.bottom - clientRect.top;

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

	myDirectionalLight->myShadowMap = TextureAssetHandler::CreateDepthStencil("ds", res.x, res.y);

	myDirectionalLight->ourLightBuffer.CastShadows = true;

	return myDirectionalLight;
}

Ref<PointLight> LightAssetHandler::CreatePointLight(Vector3<float> aColor, float aIntensity, float aRange, Vector3<float> aPosition)
{
	myLights.push_back(CreateRef<PointLight>());
	Ref<PointLight> result = std::dynamic_pointer_cast<PointLight>(myLights.back());

	result->Init(aColor, aIntensity);
	result->SetRange(aRange);
	result->ourLightBuffer.LightType = 1;

	return result;
}

Ref<SpotLight> LightAssetHandler::CreateSpotLight(Vector3<float> aColor, float aIntensity, float aRange, float aInner, float aOuter, Vector3<float> aPosition)
{
	myLights.push_back(CreateRef<SpotLight>());
	Ref<SpotLight> result = std::dynamic_pointer_cast<SpotLight>(myLights.back());

	result->Init(aColor, aIntensity);
	result->SetRange(aRange);
	result->SetInnerCone(aInner);
	result->SetOuterCone(aOuter);
	result->ourLightBuffer.LightType = 1;

	return result;
}

Ref<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::string& aTextureName)
{
	myEnvironmentLight = CreateRef<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);

	return myEnvironmentLight;
}
