#pragma once
#include <memory>
#include <vector>
#include <string>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "EnvironmentLight.h"

class LightAssetHandler
{
public:
	static Ref<DirectionalLight> CreateDirectionalLight(Vector3<float> aColor, float aIntensity, Vector3<float> aPosition, Vector3<float> aRotation);
	static Ref<PointLight> CreatePointLight(Vector3<float> aColor, float aIntensity, float aRange, float fov, float aspectRatio);
	static Ref<SpotLight> CreateSpotLight(Vector3<float> aColor, float aIntensity, float aRange, float aInner, float aOuter, float fov, float aspectRatio);
	static Ref<EnvironmentLight> CreateEnvironmentLight(const std::string& aTextureName);

private:
	static inline std::vector<Ref<Light>> myLights{};
	static inline Ref<DirectionalLight> myDirectionalLight;
	static inline Ref<EnvironmentLight> myEnvironmentLight;
};