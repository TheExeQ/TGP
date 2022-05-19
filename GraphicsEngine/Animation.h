#pragma once
#include <string>
#include <vector>
#include "Math/Matrix4x4.hpp"

struct Animation
{
	struct Frame
	{
		std::vector<CommonUtilities::Matrix4x4<float>> LocalTransforms;
	};

	std::vector<Frame> Frames;
	
	unsigned int Length;
	float Duration;
	float FramesPerSecond;
	std::string Name;
};