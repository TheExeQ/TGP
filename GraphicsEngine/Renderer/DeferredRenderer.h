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
	bool Init();
	void GenereteGBuffer(
		const std::shared_ptr<Camera>& aCamera,
		const std::vector<std::shared_ptr<ModelInstance>>& aModelList,
		float aDeltaTime, float aTotalTime);
	void Render(
		const std::shared_ptr<Camera>& aCamera,
		const std::shared_ptr<DirectionalLight>& aDirectionalLight, 
		const std::shared_ptr<EnvironmentLight>& aEnvironmentLight,
		float aDeltaTime, float aTotalTime);

private:
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;
	MaterialBufferData myMaterialBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;

	ComPtr<ID3D11PixelShader> myGBufferPS;
	ComPtr<ID3D11VertexShader> myFullscreenVS;
	ComPtr<ID3D11PixelShader> myEnvironmentPS;
};