#pragma once
#include "Renderer/ModelAssetHandler.h"
#include "Core/Base.h"
#include "Core/UUID.h"

#include <entt/entt.hpp>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>
#include <functional>

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
	std::vector<Entity> CullLights(Entity camera);

	void ParentEntity(Entity aChild, Entity aParent);
	void UnparentEntity(Entity aEntity);

	void ForEach(std::function<void(entt::entity)> aFunc);

	void SetMainCamera(Entity aCamera);
	Entity GetMainCamera() const;

	const entt::registry& GetRegistry() const { return myRegistry; };

	void LockGuard() 
	{
		myGuard = CreateRef<std::lock_guard<std::mutex>>(myRegistryMutex);
	};

	void UnlockGuard()
	{
		myGuard.reset();
	}

	static void SetActiveScene(Ref<Scene> aScene) { myActiveScene = aScene; };
	static Ref<Scene>& GetActiveScene() { return myActiveScene; };

	static void LockActiveScene() { myActiveSceneMutex.lock(); };
	static void UnlockActiveScene() { myActiveSceneMutex.unlock(); };

private:
	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;

	Ref<std::lock_guard<std::mutex>> myGuard;
	std::mutex myRegistryMutex;
	entt::registry myRegistry;
	EntityMap myEnttMap;
	entt::entity myMainCameraEntity;

	inline static std::mutex myActiveSceneMutex;
	inline static Ref<Scene> myActiveScene = nullptr;
};

