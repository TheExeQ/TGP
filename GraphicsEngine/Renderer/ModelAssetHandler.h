#pragma once
#include "Core/Base.h"
#include <unordered_map>
#include <memory>
#include <string>

class Model;
class ModelInstance;
class Material;

class ModelAssetHandler
{
public:
	bool Init();
	
	static Ref<ModelInstance> GetModelInstance(const std::string& name);
	static Ref<Model> GetModel(const std::string& name);
	static bool LoadModel(const std::string& someFilePath);
	static bool LoadAnimation(const std::string& aModelName, const std::string& someFilePath);
	
private:
	bool InitUnitCube();
	
	static std::unordered_map<std::string, Ref<Model>> myModelRegistry;
	static std::unordered_map<std::string, Ref<Material>> myMaterialRegistry;
};

