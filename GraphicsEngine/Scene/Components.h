#pragma once
#include "Core/UUID.h"
#include "Math/Vector3.hpp"
#include "Math/Matrix4x4.hpp"
#include "Renderer/ModelInstance.h"
#include "Renderer/Camera.h"
#include "Renderer/ParticleSystem.h"

#include <entt/entt.hpp>


struct TagComponent
{
	std::string name;
};

struct IDComponent
{
	TGA::UUID uuid = TGA::UUID();
};

struct TransformComponent
{
	Vector3<float> myPosition;
	Vector3<float> myRotation;
	Vector3<float> myScale;
	Matrix4x4<float> myMatrix;
};

struct ModelComponent
{
	ModelInstance mdlInstance;
};

struct CameraComponent
{
	Camera camera;
};

struct ParticleSystemComponent
{
	ParticleSystem system;
};