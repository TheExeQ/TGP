#pragma once
#include "Core/UUID.h"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"
#include "Renderer/ModelInstance.h"
#include "Renderer/Camera.h"
#include "Renderer/ParticleSystem.h"
#include "Renderer/Light.h"

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
	Vector3f position;
	Vector3f rotation;
	Vector3f scale = { 1.f, 1.f, 1.f };

	Matrix4 GetTransform() 
	{
		Matrix4 transform;
		transform = Matrix4::Rotate(transform, rotation);
		transform = Matrix4::Scale(transform, scale);
		transform = Matrix4::Translate(transform, position);
		return transform;
	};
};

struct ModelComponent
{
	ModelInstance modelInstance;
};

struct CameraComponent
{
	Camera camera;
};

struct ParticleSystemComponent
{
	ParticleSystem system;
};

struct LightComponent
{
	Light light;
};
