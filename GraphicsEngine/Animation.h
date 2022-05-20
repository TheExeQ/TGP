#pragma once
#include <string>
#include <vector>
#include "Math/Matrix4x4.hpp"

enum class eAnimationState
{
	Playing,
	Paused,
	Stopped
};

struct Animation
{
	struct Frame
	{
		std::vector<CommonUtilities::Matrix4x4<float>> LocalTransforms;
	};

	std::vector<Frame> Frames;
	size_t CurrentFrame;
	eAnimationState State;

	unsigned int Length;
	float Duration;
	float FramesPerSecond;
	std::string Name;
};