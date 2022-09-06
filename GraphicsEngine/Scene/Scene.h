#pragma once
#include "Renderer/ModelAssetHandler.h"
#include "Core/Base.h"
#include "Core/UUID.h"

#include <entt/entt.hpp>
#include <unordered_map>
#include <memory>
#include <vector>

using EntityMap = std::unordered_map<TGA::UUID, entt::entity>;

class Camera;
class ParticleSystem;
class Entity;

class Scene
{
public:
	Scene();
	~Scene() = default;

	Entity CreateEntity(const char* aName, Ref<Scene> aScene);
	Entity CreateEntityWithID(TGA::UUID aID, const char* aName, Ref<Scene> aScene);
	Entity GetEntityFromUUID(TGA::UUID aID);

	void DestroyEntity(Entity aEntity);
	
	std::vector<Entity> CullModels(Entity camera);
	std::vector<Entity> CullParticles(Entity camera);

	void SetMainCamera(Entity aCamera);
	Entity GetMainCamera() const;

	static void SetActiveScene(Ref<Scene> aScene) { myActiveScene = aScene; };
	static Ref<Scene> GetActiveScene() { return myActiveScene; };

private:
	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;

	entt::registry myRegistry;
	EntityMap myEnttMap;
	entt::entity myMainCameraEntity;

	inline static Ref<Scene> myActiveScene = nullptr;
};
