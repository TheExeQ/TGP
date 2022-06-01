#include "ModelInstance.h"
#include "GraphicsEngine.h"
#include <iostream>

void ModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myModel = aModel;
}

void ModelInstance::Update()
{
	if (myCurrentAnimation.State == eAnimationState::Playing)
	{
		float frameTime = 1.f / myCurrentAnimation.FramesPerSecond;
		myCurrentAnimation.Timer += GraphicsEngine::GetTimer().GetDeltaTime();

		if (myCurrentAnimation.Timer > (frameTime * myCurrentAnimation.CurrentFrame))
		{
			if (myCurrentAnimation.CurrentFrame < myCurrentAnimation.Length - 1)
			{
				myCurrentAnimation.CurrentFrame++;
			}
			else
			{
				if (myCurrentAnimation.Looping)
				{
					myCurrentAnimation.CurrentFrame = 1;
					myCurrentAnimation.Timer = 0.f;
				}
			}
		}

		Matrix4x4<float> identity;

		UpdateAnimationHierarchy(myCurrentAnimation.CurrentFrame, 0, myCurrentAnimation, identity, &myModel->GetBoneTransforms()[0]);
	}
}

void ModelInstance::UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned int aBoneIdx, const Animation anAnimation, Matrix4x4<float>& aParentTransform, Matrix4x4<float>* outBoneTransform)
{
	if (myCurrentAnimation.Frames.size() <= 0) return;

	auto bones = myModel->GetSkeleton()->Bones;
	outBoneTransform[aBoneIdx] = aParentTransform * anAnimation.Frames[anAnimation.CurrentFrame].LocalTransforms[aBoneIdx];

	for (auto bone : bones[aBoneIdx].Children)
	{
		UpdateAnimationHierarchy(aCurrentFrame, bone, anAnimation, myModel->GetBoneTransforms()[aBoneIdx], outBoneTransform);
	}
	
	outBoneTransform[aBoneIdx] *= bones[aBoneIdx].BindPoseInverse;
}

