#pragma once
#include "Scene.h"
#include "Model.h"
#include <memory>
#include "Math/Matrix4x4.hpp"

enum class eAnimationState
{
	Playing,
	Paused,
	Stopped
};

class ModelInstance : public SceneObject
{
public:
	ModelInstance() = default;

	void Init(std::shared_ptr<Model> aModel);

	void Update(float aDeltaTime);

	FORCEINLINE std::shared_ptr<Model> GetModel() { return myModel; }
	FORCEINLINE Model::ModelData const& GetModelData(uint16_t anIndex) const { return myModel->GetModelData(anIndex); }
	FORCEINLINE uint16_t GetNumMeshes() const { return myModel->GetNumMeshes(); }

private:
	std::shared_ptr<Model> myModel;
	Animation myCurrentAnimation;
	eAnimationState myAnimationState;

	CommonUtilities::Matrix4x4<float> myBoneTransforms[128];
	
	void UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned int aBoneIdx, const Animation anAnimation, Matrix4x4<float>& aParentTransform, Matrix4x4<float>& outBoneTransform);
};