#pragma once
#include "Math/Vector.hpp"

typedef CommonUtilities::Vector3<float> Vector3f;
typedef CommonUtilities::Vector4<float> Vector4f;

struct ParticleVertex
{
	Vector4f Position = { 0.f, 0.f, 0.f, 1.f };
	Vector4f Color = { 0.f, 0.f, 0.f, 0.f };
	Vector3f Velocity = { 0.f, 0.f, 0.f };
	Vector3f Scale = { 1.f, 1.f, 1.f };
	float LifeTime = 0.f;
};

