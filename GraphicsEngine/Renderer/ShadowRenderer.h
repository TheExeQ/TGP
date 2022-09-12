#pragma once
#include "ConstantBufferTypes.h"
#include "Scene/Entity.h"

#include <vector>

class ShadowRenderer
{
public:
	bool Init();
	void Render(Entity& aLight, std::vector<Entity>& aModelList);

private:
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
};