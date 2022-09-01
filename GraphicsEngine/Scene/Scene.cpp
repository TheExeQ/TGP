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

std::vector<Entity> Scene::CullModels(Entity camera) const
{
	std::vector<Entity> visibleEntity;

	const auto& view = myRegistry.view<ModelComponent>();

	for (auto& entity : view)
	{
		auto mdlInst = myRegistry.get<ModelComponent>(entity).mdlInstance;
		mdlInst.Update();
		visibleEntity.push_back(Entity(entity, myActiveScene));
	}

	return visibleEntity;
}

std::vector<Entity> Scene::CullParticles(Entity camera) const
{
	std::vector<Entity> visibleParticles;

	//for (auto& sceneObject : mySceneObjects)
	//{
	//	auto particleSystem = dynamic_pointer_cast<ParticleSystem>(sceneObject);
	//	if (particleSystem)
	//	{
	//		particleSystem->Update(GraphicsEngine::GetTimer().GetDeltaTime());
	//		visibleParticles.push_back(particleSystem);
	//	}
	//}

	return visibleParticles;
}

void Scene::SetMainCamera(Entity aCamera)
{
	myMainCameraEntity = (entt::entity)aCamera;
}

Entity Scene::GetMainCamera() const
{
	return Entity(myMainCameraEntity, myActiveScene);
}
