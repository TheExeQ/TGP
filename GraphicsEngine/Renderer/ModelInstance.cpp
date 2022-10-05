#include "ModelInstance.h"
#include "Core/GraphicsEngine.h"
#include <iostream>

ModelInstance::ModelInstance(const ModelInstance& rhs)
{
	myModel = rhs.myModel;
	myCurrentAnimation = rhs.myCurrentAnimation;

	myInstanceBuffer = rhs.myInstanceBuffer;
	myRenderedInstances = rhs.myRenderedInstances;
}

void ModelInstance::Init(Ref<Model> aModel)
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

void ModelInstance::AddRenderedInstance(const Matrix4x4<float>& aTransform)
{
	RenderedInstanceData aData;
	aData.World = aTransform;
	myRenderedInstances.push_back(aData);
}

bool ModelInstance::UpdateInstanceBuffer()
{
	HRESULT result = E_FAIL;

	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(RenderedInstanceData) * myRenderedInstances.size());
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = myRenderedInstances.data();

	result = DX11::myDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, myInstanceBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
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

