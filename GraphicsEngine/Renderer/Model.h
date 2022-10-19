#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <vector>
#include <array>

#include "Math/Matrix4x4.hpp"
#include "Scene/Scene.h"
#include "Skeleton.h"

class Material;

using namespace Microsoft::WRL;

class Model
{
public:
	struct ModelData
	{
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;

		ComPtr<ID3D11InputLayout> myInputLayout;
		ComPtr<ID3D11VertexShader> myVS;
		ComPtr<ID3D11PixelShader> myPS;

		UINT myPrimitiveTopology = 0;
		UINT myVertexCount = 0;
		UINT myIndexCount = 0;
		UINT myStride = 0;
		UINT myOffset = 0;

		Ref<Material> myMaterial;
	};
	
	void Init(std::vector<ModelData>& someModelData, const std::string& aName);
	void Init(std::vector<ModelData>& someModelData, const std::string& aName, Skeleton& aSkeleton);
	FORCEINLINE ModelData const& GetModelData(uint16_t aIndex) const { return myData[aIndex]; }
	FORCEINLINE uint16_t GetNumMeshes() const { return (uint16_t)myData.size(); }
	FORCEINLINE std::string GetName() const { return myName; }
	FORCEINLINE const Skeleton* GetSkeleton() const { return &mySkeleton; }
	FORCEINLINE std::array<CommonUtilities::Matrix4x4<float>, 128>& GetBoneTransforms() { return myBoneTransforms; }
	FORCEINLINE bool HasBones() { return (mySkeleton.GetRoot()) ? true : false; };

	void AddAnimation(Animation aAnimation);
	void SetMaterial(Ref<Material> aMaterial);

private:
	std::array<CommonUtilities::Matrix4x4<float>, 128> myBoneTransforms;

	std::vector<ModelData> myData;
	std::string myName;

	Skeleton mySkeleton;
};

