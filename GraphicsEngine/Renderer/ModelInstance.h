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
	ModelInstance(const ModelInstance& rhs);

	struct RenderedInstanceData
	{
		Matrix4x4<float> World;
	};

	void Init(Ref<Model> aModel);

	void Update();

	void AddRenderedInstance(const Matrix4x4<float>& aTransform);
	bool UpdateInstanceBuffer();
	void ClearRenderedInstance() { myRenderedInstances.clear(); };

	FORCEINLINE Ref<Model> GetModel() const { return myModel; }
	FORCEINLINE Model::ModelData const& GetModelData(uint16_t anIndex) const { return myModel->GetModelData(anIndex); }
	FORCEINLINE uint16_t GetNumMeshes() const { return myModel->GetNumMeshes(); }
	FORCEINLINE void SetAnimation(const std::string& anAnimationName) { myCurrentAnimation = myModel->GetSkeleton()->Animations.at("../Assets/" + anAnimationName); }
	FORCEINLINE void SetAnimationState(const eAnimationState& anAnimationState) { myCurrentAnimation.State = anAnimationState; }

	FORCEINLINE bool HasRenderedInstances() const { return !myRenderedInstances.empty(); }
	FORCEINLINE uint32_t GetNumOfInstances() const { 
		return (uint16_t)myRenderedInstances.size(); }
	FORCEINLINE ComPtr<ID3D11Buffer> GetInstanceBuffer() const { return myInstanceBuffer; }

private:
	Ref<Model> myModel;
	Animation myCurrentAnimation;

	ComPtr<ID3D11Buffer> myInstanceBuffer;
	std::vector<RenderedInstanceData> myRenderedInstances;

	void UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned int aBoneIdx, const Animation anAnimation, Matrix4x4<float>& aParentTransform, Matrix4x4<float>* outBoneTransform);
};