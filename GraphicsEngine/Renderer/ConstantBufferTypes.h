#pragma once
#include <vector>
#include "Math/Matrix4x4.hpp"
#include "Math/Vector.hpp"
#include "Light.h"

using namespace CommonUtilities;

constexpr uint32_t MAX_FORWARD_LIGHTS = 8;

struct FrameBufferData
{
	CommonUtilities::Matrix4x4<float> View;
	CommonUtilities::Matrix4x4<float> Projection;
	CommonUtilities::Vector3<float> CamTranslation;
	unsigned int RenderMode = 0;
	CommonUtilities::Vector2<float> Resolution;
	float nearPlane;
	float farPlane;
	std::array<CommonUtilities::Vector4<float>, 4> FrustumCorners;
};

struct ObjectBufferData
{
	CommonUtilities::Matrix4x4<float> World;
	CommonUtilities::Matrix4x4<float> BoneData[128];
	alignas(16) bool HasBones = false;
};

struct MaterialBufferData
{
	CommonUtilities::Vector3<float> Albedo;
	float Padding;
};

struct SceneLightBufferData
{
	Light::LightBufferData DirectionalLight;
	Light::LightBufferData Lights[MAX_FORWARD_LIGHTS];
	uint32_t numLights;
	CommonUtilities::Vector3<float> Padding;
};