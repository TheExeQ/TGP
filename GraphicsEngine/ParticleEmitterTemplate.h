#pragma once
#include <string>
#include "Math/Vector.hpp"

typedef CommonUtilities::Vector3<float> Vector3f;
typedef CommonUtilities::Vector4<float> Vector4f;

struct EmitterSettingsData
{
	float SpawnRate = 1.f;
	float SpawnAngle = 0.f;
	float LifeTime = 1.f;
	Vector3f StartVelocity = { 0.f, 0.f, 0.f };
	Vector3f EndVelocity = { 0.f, 0.f, 0.f };
	float GravityScale = 0.f;
	float StartSize = 10.f;
	float EndSize = 20.f;
	Vector4f StartColor = { 1.f, 1.f, 1.f, 1.f };
	Vector4f EndColor = { 0.f, 0.f, 0.f, 1.f };
};

struct ParticleEmitterTemplate
{
	std::string Path;
	EmitterSettingsData EmitterSettings;
};

struct ParticleSystemTemplate
{
	std::string Path;
	std::vector<ParticleEmitterTemplate> EmitterTemplates;
};

