#pragma once
#include "Core/Base.h"
#include "ConstantBufferTypes.h"
#include "Scene/Entity.h"

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
		Entity aCamera,
		std::vector<Entity>& aModelList);
	void Render(
		Entity aCamera,
		std::vector<Entity>& aLightList,
		const Ref<DirectionalLight>& aDirectionalLight, 
		const Ref<EnvironmentLight>& aEnvironmentLight);

private:
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;
	MaterialBufferData myMaterialBufferData;
	SceneLightBufferData mySceneLightBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> mySceneLightBuffer;

	ComPtr<ID3D11PixelShader> myGBufferPS;
	ComPtr<ID3D11VertexShader> myFullscreenVS;
	ComPtr<ID3D11PixelShader> myEnvironmentPS;
};