#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Math/Matrix4x4.hpp"
#include "Animation.h"

struct Skeleton
{
	std::string Name;
	struct Bone
	{
		CommonUtilities::Matrix4x4<float> BindPoseInverse;
		int Parent = 0;
		std::vector<unsigned int> Children;
		std::string Name;
	};
	
	std::vector<Bone> Bones;
	std::unordered_map<std::string, size_t> BoneNameToIndex;
	std::vector<std::string> BoneNames;
	
	std::unordered_map<std::string, Animation> Animations;
	
	inline const Bone* GetRoot() const
	{
		if (!Bones.empty())
		{
			return &Bones[0];
		}
		return nullptr;
	}
};