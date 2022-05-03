#include "GraphicsEngine.pch.h"
#include "ForwardRenderer.h"
#include "Camera.h"
#include "Model.h"
#include "DX11.h"

bool ForwardRenderer::Initialize()
{
	HRESULT result = S_FALSE;
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(FrameBufferData);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	
	result = DX11::myDevice->CreateBuffer(&bufferDesc, nullptr, myFrameBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	bufferDesc.ByteWidth = sizeof(ObjectBufferData);
	result = DX11::myDevice->CreateBuffer(&bufferDesc, nullptr, myObjectBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

void ForwardRenderer::Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE bufferData;
	
	myFrameBufferData.View = Matrix4x4<float>::GetFastInverse(aCamera->GetTransform().myMatrix);
	myFrameBufferData.Projection = aCamera->GetProjectionMatrix();
	
	result = DX11::myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::myContext->Unmap(myFrameBuffer.Get(), 0);
	
	for (const std::shared_ptr<Model>& model : aModelList)
	{
		const Model::ModelData& modelData = model->GetModelData();
		DX11::myContext->IASetVertexBuffers(0, 1, modelData.myVertexBuffer.GetAddressOf(), &modelData.myStride, &modelData.myOffset);
		DX11::myContext->IASetIndexBuffer(modelData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		
		DX11::myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(modelData.myPrimitiveTopology));
		DX11::myContext->IASetInputLayout(modelData.myInputLayout.Get());
		
		DX11::myContext->VSSetShader(modelData.myVS.Get(), nullptr, 0);
		DX11::myContext->PSSetShader(modelData.myPS.Get(), nullptr, 0);
		
		DX11::myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		DX11::myContext->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		
		DX11::myContext->DrawIndexed(modelData.myIndexCount, 0, 0);
	}
}
