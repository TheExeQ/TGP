#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <vector>
#include "Scene.h"
#include "Skeleton.h"

class Material;

using namespace Microsoft::WRL;

class Model : public SceneObject
{
public:
	struct ModelData
	{
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;

		ComPtr<ID3D11InputLayout> myInputLayout;
		ComPtr<ID3D11VertexShader> myVS;
		ComPtr<ID3D11PixelShader> myPS;

		UINT myPrimitiveTopology;
		UINT myVertexCount;
		UINT myIndexCount;
		UINT myStride;
		UINT myOffset;

		std::shared_ptr<Material> myMaterial;
	};
	
	void Init(std::vector<ModelData>& someModelData, const std::string& aName);
	void Init(std::vector<ModelData>& someModelData, const std::string& aName, Skeleton& aSkeleton);
	FORCEINLINE ModelData const& GetModelData(uint16_t aIndex) const { return myData[aIndex]; }
	FORCEINLINE uint16_t GetNumMeshes() const { return myData.size(); }
	FORCEINLINE std::string GetName() const { return myName; }
	FORCEINLINE const Skeleton* GetSkeleton() const { return &mySkeleton; }

	void AddAnimation(Animation aAnimation);

private:
	std::vector<ModelData> myData;
	std::string myName;

	Skeleton mySkeleton;
};

