#pragma once
#include "Core/Base.h"
#include "Components.h"
#include "Scene.h"

#include <entt/entt.hpp>

class Entity
{
public:
	Entity();
	Entity(TGA::UUID aID, Ref<Scene> aScene);
	Entity(entt::entity aEntityHandle, Ref<Scene> aScene);
	Entity(const Entity& aEntity);

	TGA::UUID GetUUID();
	bool IsValid();

	TGA::UUID ParentUUID();
	bool HasParent();

	std::vector<TGA::UUID> ChildrenUUIDs();

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		if (HasComponent<T>()) { return GetComponent<T>(); }
		return myScene->myRegistry.emplace<T>(myEntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		return myScene->myRegistry.get<T>(myEntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		if (!HasComponent<T>()) { return; }
		myScene->myRegistry.remove<T>(myEntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return (myScene->myRegistry.valid(myEntityHandle)) ? myScene->myRegistry.all_of<T>(myEntityHandle) : false;
	}

	operator entt::entity() const { return myEntityHandle; }
	operator uint32_t() const { return (uint32_t)myEntityHandle; }

	bool operator==(const Entity& other) const
	{
		return myEntityHandle == other.myEntityHandle && myScene == other.myScene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

private:
	entt::entity myEntityHandle = entt::null;
	Ref<Scene> myScene;
};