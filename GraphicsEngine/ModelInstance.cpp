#include "ModelInstance.h"

void ModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myModel = aModel;
}

void ModelInstance::Update(float aDeltaTime)
{
	if (myCurrentAnimation.State == eAnimationState::Playing)
	{
		Matrix4x4<float> identity;
		
		UpdateAnimationHierarchy(myCurrentAnimation.CurrentFrame, 0, myCurrentAnimation, identity, &myBoneTransforms[0]);
		if (myCurrentAnimation.CurrentFrame < myCurrentAnimation.Length - 1)
		{
			myCurrentAnimation.CurrentFrame++;
		}
		else
		{
			myCurrentAnimation.CurrentFrame = 1;
		}
	}
}

void ModelInstance::UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned int aBoneIdx, const Animation anAnimation, Matrix4x4<float>& aParentTransform, Matrix4x4<float>* outBoneTransform)
{
	if (myCurrentAnimation.Frames.size() <= 0) return;
	myBoneTransforms[2] =
		aParentTransform 
		* myCurrentAnimation.Frames[aCurrentFrame].LocalTransforms[0]
		* myCurrentAnimation.Frames[aCurrentFrame].LocalTransforms[1]
		* myCurrentAnimation.Frames[aCurrentFrame].LocalTransforms[2];
}

