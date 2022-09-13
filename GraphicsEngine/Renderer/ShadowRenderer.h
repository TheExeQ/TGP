#pragma once
#include "ConstantBufferTypes.h"
#include "Core/Base.h"
#include "Scene/Entity.h"
#include "DirectionalLight.h"

#include <vector>

class ShadowRenderer
{
public:
	bool Init();
	void Render(std::vector<Entity>& aLight, Ref<DirectionalLight> aDirectionalLight, std::vector<Entity>& aModelList);

private:
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
};