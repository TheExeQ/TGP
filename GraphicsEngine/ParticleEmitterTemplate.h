#pragma once
#include <string>
#include "Math/Vector.hpp"

typedef CommonUtilities::Vector3<float> Vector3f;
typedef CommonUtilities::Vector4<float> Vector4f;

struct EmitterSettingsData
{
	float SpawnRate = 1.f;
	float SpawnAngle = 0.f;
	float LifeTime;
	Vector3f StartVelocity;
	Vector3f EndVelocity;
	float GravityScale;
	float StartSize;
	float EndSize;
	Vector4f StartColor;
	Vector4f EndColor;
};

struct ParticleEmitterTemplate
{
	std::string Path;
	EmitterSettingsData EmitterSettings;
};

struct ParticleSystemTemplate
{
	std::string Path;
	std::vector<std::string> EmitterFiles;
};

