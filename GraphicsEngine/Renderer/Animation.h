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
	size_t CurrentFrame = 1;
	eAnimationState State;

	bool Looping = true;
	float Timer = 0.f;
	unsigned int Length = 0;
	float Duration = 0.f;
	float FramesPerSecond = 0.f;
	std::string Name = "";
};