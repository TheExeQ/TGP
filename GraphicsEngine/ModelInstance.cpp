#include "ModelInstance.h"

void ModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myModel = aModel;
}

void ModelInstance::Update(float aDeltaTime)
{
	if (myAnimationState == eAnimationState::Playing)
	{
		Matrix4x4<float> identity;
		
		// TODO: Continue on page 29
	}
}

void ModelInstance::UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned int aBoneIdx, const Animation anAnimation, Matrix4x4<float>& aParentTransform, Matrix4x4<float>& outBoneTransform)
{

}

