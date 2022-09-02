#pragma once
#include "Core/Base.h"
#include "Scene.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

class SceneSerializer
{
public:
	SceneSerializer(Ref<Scene> aScene) { myScene = aScene; };
	~SceneSerializer() = default;

	void Serialize(const char* aFileName);
	bool Deserialize(const char* aFileName);

private:
	void SerializeEntity(YAML::Emitter& outEmitter, Entity aEntity);

	Ref<Scene> myScene;
};