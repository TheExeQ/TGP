#pragma once
#include <vector>
#include "Math/Matrix4x4.hpp"
#include "Math/Vector.hpp"

using namespace CommonUtilities;

struct FrameBufferData
{
	CommonUtilities::Matrix4x4<float> View;
	CommonUtilities::Matrix4x4<float> Projection;
	CommonUtilities::Vector3<float> CamTranslation;
	unsigned int RenderMode = 0;
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
	float padding;
};
