#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

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

		UINT myPrimitiveTopology;
		UINT myVertexCount;
		UINT myIndexCount;
		UINT myStride;
		UINT myOffset;
	};
	
	void Init(ModelData& someModelData, const std::string& aPath);
	FORCEINLINE ModelData const& GetModelData() const { return myData; }
	FORCEINLINE std::string GetName() const { return myName; }

private:
	ModelData myData;
	std::string myName;
};

