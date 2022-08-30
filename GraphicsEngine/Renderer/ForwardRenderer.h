#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include "Math/Matrix4x4.hpp"
#include "Math/Vector.hpp"

class Camera;
class ModelInstance;
class DirectionalLight;
class EnvironmentLight;
class ParticleSystem;

using namespace CommonUtilities;
using namespace Microsoft::WRL;

class ForwardRenderer
{
public:
	bool Initialize();
	void RenderModels(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ModelInstance>>& aModelList,
		const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<EnvironmentLight>& aEnvironmentLight);
	void RenderParticles(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<ParticleSystem>>& aParticleSystemList);

private:
	
	struct FrameBufferData
	{
		Matrix4x4<float> View;
		Matrix4x4<float> Projection;
		Vector3<float> CamTranslation;
		unsigned int RenderMode = 0;
	} myFrameBufferData;
	
	struct ObjectBufferData
	{
		Matrix4x4<float> World;
		Matrix4x4<float> BoneData[128];
		alignas(16) bool HasBones = false;
	} myObjectBufferData;
	
	struct MaterialBufferData
	{
		Vector3<float> Albedo;
		float padding;
	} myMaterialBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;
};

