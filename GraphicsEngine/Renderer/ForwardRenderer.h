#pragma once
#include "ConstantBufferTypes.h"
#include "Core/Base.h"
#include "Scene/Entity.h"

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
	void RenderModels(Entity aCamera, std::vector<Entity>& aModelList,
		const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& aEnvironmentLight);
	void RenderParticles(Entity aCamera, std::vector<Entity>& aParticleSystemList);

private:
	
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;
	MaterialBufferData myMaterialBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;
};

