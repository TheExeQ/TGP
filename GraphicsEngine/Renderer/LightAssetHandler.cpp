#include "LightAssetHandler.h"
#include "Math/Matrix.hpp"
#include "TextureAssetHandler.h"

Ref<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3<float> aColor, float aIntensity, Vector3<float> aRotation)
{
	myDirectionalLight = CreateRef<DirectionalLight>();
	myDirectionalLight->Init(aColor, aIntensity);
	
	Matrix4x4<float> lightRotation = Matrix4::Rotate(lightRotation, aRotation);

	myDirectionalLight->ourlightBuffer.Direction = Vector3<float>(lightRotation(3, 1), lightRotation(3, 2), lightRotation(3, 3));

	return myDirectionalLight;
}

Ref<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::string& aTextureName)
{
	myEnvironmentLight = CreateRef<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);

	return myEnvironmentLight;
}
