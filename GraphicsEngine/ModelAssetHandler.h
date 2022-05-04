#pragma once
#include <unordered_map>
#include <memory>
#include <string>

class Model;

class ModelAssetHandler
{
public:
	bool Init();
	
	std::shared_ptr<Model> GetModel(const std::string& name) const;
	
private:
	bool InitUnitCube();
	
	static std::unordered_map<std::string, std::shared_ptr<Model>> myModelRegistry;
};

