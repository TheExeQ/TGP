#pragma once
#include <unordered_map>
#include <memory>
#include <string>

class Model;
class ModelInstance;

class ModelAssetHandler
{
public:
	bool Init();
	
	std::shared_ptr<ModelInstance> GetModelInstance(const std::string& name) const;
	bool LoadModel(const std::string& someFilePath);
	
private:
	bool InitUnitCube();
	
	static std::unordered_map<std::string, std::shared_ptr<Model>> myModelRegistry;
};

