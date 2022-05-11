#pragma once
#include "Scene.h"
#include "Model.h"
#include <memory>

class ModelInstance : public SceneObject
{
public:
	ModelInstance() = default;

	void Init(std::shared_ptr<Model> aModel);
	FORCEINLINE Model::ModelData const& GetModelData(uint16_t anIndex) const { return myModel->GetModelData(anIndex); }
	FORCEINLINE uint16_t GetNumMeshes() const { return myModel->GetNumMeshes(); }

private:
	std::shared_ptr<Model> myModel;
};