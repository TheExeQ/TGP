#include "Core/GraphicsEngine.pch.h"
#include "Core/GraphicsEngine.h"
#include "Renderer/Camera.h"
#include "Scene/Scene.h"
#include "Renderer/ModelInstance.h"
#include "Renderer/ParticleSystem.h"
#include "Scene/Entity.h"

#define PI 3.14f

Entity Scene::GetEntityFromUUID(TGA::UUID aID)
{
	if (auto it = myEnttMap.find(aID); it != myEnttMap.end())
	{
		return Entity(it->second, myActiveScene);
	}
	return Entity();
}

void Scene::DestroyEntity(Entity aEntity)
{
	myRegistry.destroy(aEntity);
}

Entity Scene::CreateEntity(const char* aName, Ref<Scene> aScene)
{
	if (!aScene) { return Entity(); }
	Entity entity(myRegistry.create(), aScene);
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>().name = aName;
	auto uuid = entity.AddComponent<IDComponent>().uuid;
	myEnttMap[uuid] = (entt::entity)entity;
	return entity;
}
Entity Scene::CreateEntityWithID(TGA::UUID aID, const char* aName, Ref<Scene> aScene)
{
	if (!aScene) { return Entity(); }
	Entity entity(myRegistry.create(), aScene);
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>().name = aName;
	entity.AddComponent<IDComponent>().uuid = aID;
	myEnttMap[aID] = (entt::entity)entity;
	return entity;
}

Scene::Scene()
{
	myRegistry.clear();
}

std::vector<Entity> Scene::CullModels(Entity camera)
{
	std::vector<Entity> visibleEntity;

	const auto& view = myRegistry.view<ModelComponent>();

	for (auto& entity : view)
	{
		ModelComponent& mdl = myRegistry.get<ModelComponent>(entity);
		mdl.modelInstance.Update();
		visibleEntity.push_back(Entity(entity, myActiveScene));
	}

	return visibleEntity;
}

std::vector<Entity> Scene::CullParticles(Entity camera)
{
	std::vector<Entity> visibleEntity;

	const auto& view = myRegistry.view<ParticleSystemComponent>();

	for (auto& entity : view)
	{
		ParticleSystemComponent& particleSystem = myRegistry.get<ParticleSystemComponent>(entity);
		particleSystem.system.Update(GraphicsEngine::GetTimer().GetDeltaTime());
		visibleEntity.push_back(Entity(entity, myActiveScene));
	}

	return visibleEntity;
}

void Scene::SetMainCamera(Entity aCamera)
{
	myMainCameraEntity = (entt::entity)aCamera;
}

Entity Scene::GetMainCamera() const
{
	return Entity(myMainCameraEntity, myActiveScene);
}
