#pragma once
#include "Core/UUID.h"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"
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
	Vector3f myPosition;
	Vector3f myRotation;
	Vector3f myScale = { 1.f, 1.f, 1.f };

	Matrix4 GetTransform() 
	{
		Matrix4 transform;
		transform = Matrix4::Rotate(transform, myRotation);
		transform = Matrix4::Scale(transform, myScale);
		transform = Matrix4::Translate(transform, myPosition);
		return transform;
	};
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