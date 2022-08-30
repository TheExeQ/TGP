#pragma once
#include "ConstantBufferTypes.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

using namespace Microsoft::WRL;

class Camera;
class ModelInstance;
class DirectionalLight;
class EnvironmentLight;
class ParticleSystem;

class DeferredRenderer
{
public:
	DeferredRenderer();
	void GenereteGBuffer(
		const std::shared_ptr<Camera>& aCaemra,
		const std::vector<std::shared_ptr<ModelInstance>>& aModelList,
		float aDeltaTime, float aTotalTime);

private:
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;

	ComPtr<ID3D11PixelShader> myGBufferPS;
};