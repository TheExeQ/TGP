#include "LightAssetHandler.h"
#include "Math/Matrix.hpp"
#include "TextureAssetHandler.h"

std::shared_ptr<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3<float> aColor, float aIntensity, Vector3<float> aRotation)
{
	myDirectionalLight = std::make_shared<DirectionalLight>();
	myDirectionalLight->Init(aColor, aIntensity);
	
	Matrix4x4<float> lightRotation = 
		Matrix4x4<float>::CreateRotationAroundX(aRotation.x) *
		Matrix4x4<float>::CreateRotationAroundY(aRotation.y) *
		Matrix4x4<float>::CreateRotationAroundZ(aRotation.z);

	myDirectionalLight->ourlightBuffer.Direction = Vector3<float>(lightRotation(3, 1), lightRotation(3, 2), lightRotation(3, 3));

	return myDirectionalLight;
}

std::shared_ptr<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::string& aTextureName)
{
	myEnvironmentLight = std::make_shared<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);

	return myEnvironmentLight;
}
