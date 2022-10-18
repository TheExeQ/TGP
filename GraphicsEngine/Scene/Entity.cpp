#include "gepch.h"
#include "Entity.h"

Entity::Entity(entt::entity aEntityHandle, Ref<Scene> aScene)
	: myEntityHandle(aEntityHandle), myScene(aScene)
{

}

Entity::Entity(const Entity& aEntity)
{
	myEntityHandle = aEntity.myEntityHandle;
	myScene = aEntity.myScene;
}

Entity::Entity()
{
}

Entity::Entity(TGA::UUID aID, Ref<Scene> aScene)
	: myScene(aScene)
{
	myEntityHandle = (entt::entity)myScene->GetEntityFromUUID(aID);
}

TGA::UUID Entity::GetUUID()
{
	return GetComponent<IDComponent>().uuid;
}

bool Entity::IsValid()
{
	if (!myScene) { return false; }
	else
	{
		return myScene->myRegistry.valid(myEntityHandle);
	}
}

TGA::UUID Entity::ParentUUID()
{
	return GetComponent<RelationshipComponent>().Parent;
}

bool Entity::HasParent()
{
	return GetComponent<RelationshipComponent>().Parent != 0;
}

bool Entity::IsChild(Entity aEntity)
{
	for (auto childUUID : ChildrenUUIDs())
	{
		Entity child(childUUID, myScene);
		if (aEntity.GetUUID() == childUUID || child.IsChild(aEntity)) { return true; };
	}
	return false;
}

std::vector<TGA::UUID> Entity::ChildrenUUIDs()
{
	return GetComponent<RelationshipComponent>().Children;
}
