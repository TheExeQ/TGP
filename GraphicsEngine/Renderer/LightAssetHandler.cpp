#include "LightAssetHandler.h"
#include "Math/Matrix.hpp"
#include "TextureAssetHandler.h"

Ref<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3<float> aColor, float aIntensity, Vector3<float> aDirection)
{
	myLights.push_back(CreateRef<DirectionalLight>());
	myDirectionalLight = std::dynamic_pointer_cast<DirectionalLight>(myLights.back());
	myDirectionalLight->Init(aColor, aIntensity);
	myDirectionalLight->ourlightBuffer.Direction = aDirection;
	myDirectionalLight->ourlightBuffer.LightType = 0;

	return myDirectionalLight;
}

Ref<PointLight> LightAssetHandler::CreatePointLight(Vector3<float> aColor, float aIntensity, float aRange, Vector3<float> aPosition)
{
	myLights.push_back(CreateRef<PointLight>());
	Ref<PointLight> result = std::dynamic_pointer_cast<PointLight>(myLights.back());

	result->Init(aColor, aIntensity);
	result->SetRange(aRange);
	result->ourlightBuffer.LightType = 1;

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
	result->ourlightBuffer.LightType = 1;

	return result;
}

Ref<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::string& aTextureName)
{
	myEnvironmentLight = CreateRef<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);

	return myEnvironmentLight;
}
