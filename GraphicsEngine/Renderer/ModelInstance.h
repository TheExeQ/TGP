#pragma once
#include "Scene/Scene.h"
#include "Model.h"
#include "Math/Matrix4x4.hpp"
#include <memory>

using namespace CommonUtilities;

class ModelInstance
{
public:
	ModelInstance() = default;

	void Init(Ref<Model> aModel);

	void Update();

	FORCEINLINE Ref<Model> GetModel() { return myModel; }
	FORCEINLINE Model::ModelData const& GetModelData(uint16_t anIndex) const { return myModel->GetModelData(anIndex); }
	FORCEINLINE uint16_t GetNumMeshes() const { return myModel->GetNumMeshes(); }
	FORCEINLINE void SetAnimation(const std::string& anAnimationName) { myCurrentAnimation = myModel->GetSkeleton()->Animations.at("../Assets/" + anAnimationName); }
	FORCEINLINE void SetAnimationState(const eAnimationState& anAnimationState) { myCurrentAnimation.State = anAnimationState; }

private:
	Ref<Model> myModel;
	Animation myCurrentAnimation;

	void UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned int aBoneIdx, const Animation anAnimation, Matrix4x4<float>& aParentTransform, Matrix4x4<float>* outBoneTransform);
};