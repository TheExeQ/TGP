#pragma once
#include "ConstantBufferTypes.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

class Camera;
class ModelInstance;
class DirectionalLight;
class EnvironmentLight;
class ParticleSystem;

using namespace Microsoft::WRL;

class ForwardRenderer
{
public:
	bool Initialize();
	void RenderModels(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ModelInstance>>& aModelList,
		const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& aEnvironmentLight);
	void RenderParticles(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ParticleSystem>>& aParticleSystemList);

private:
	
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;
	MaterialBufferData myMaterialBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;
};

