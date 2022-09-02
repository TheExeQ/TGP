#include "LightAssetHandler.h"
#include "Math/Matrix.hpp"
#include "TextureAssetHandler.h"

Ref<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3<float> aColor, float aIntensity, Vector3<float> aDirection)
{
	myDirectionalLight = CreateRef<DirectionalLight>();
	myDirectionalLight->Init(aColor, aIntensity);
	myDirectionalLight->ourlightBuffer.Direction = aDirection;

	return myDirectionalLight;
}

Ref<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::string& aTextureName)
{
	myEnvironmentLight = CreateRef<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);

	return myEnvironmentLight;
}
