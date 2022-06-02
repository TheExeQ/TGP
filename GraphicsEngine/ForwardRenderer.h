#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include "Math/Matrix4x4.hpp"
#include "Math/Vector3.hpp"

class Camera;
class Model;

using namespace CommonUtilities;
using namespace Microsoft::WRL;

class ForwardRenderer
{
public:
	bool Initialize();
	void Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList);

private:
	
	struct FrameBufferData
	{
		Matrix4x4<float> View;
		Matrix4x4<float> Projection;
	} myFrameBufferData;
	
	struct ObjectBufferData
	{
		Matrix4x4<float> World;
		//Vector3<float> Padding;
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
};

