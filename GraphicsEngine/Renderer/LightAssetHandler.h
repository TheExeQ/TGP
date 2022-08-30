#pragma once
#include <memory>
#include <vector>
#include <string>
#include "DirectionalLight.h"
#include "EnvironmentLight.h"

class LightAssetHandler
{
public:
	static std::shared_ptr<DirectionalLight> CreateDirectionalLight(Vector3<float> aColor, float aIntensity, Vector3<float> aRotation);
	static std::shared_ptr<EnvironmentLight> CreateEnvironmentLight(const std::string& aTextureName);

private:
	static inline std::vector<std::shared_ptr<Light>> myLights{};
	static inline std::shared_ptr<DirectionalLight> myDirectionalLight;
	static inline std::shared_ptr<EnvironmentLight> myEnvironmentLight;
};