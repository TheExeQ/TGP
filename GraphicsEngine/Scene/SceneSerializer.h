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

	void SerializeSettings(const char* aFileName);
	void SerializePreset(const char* aFileName);
	void Serialize(const char* aFileName);
	bool DeserializeSettings(const char* aFileName);
	bool DeserializePreset(const char* aFileName);
	bool Deserialize(const char* aFileName);

private:
	void SerializeEntity(YAML::Emitter& outEmitter, Entity aEntity);

	Ref<Scene> myScene;
};