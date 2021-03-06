#include "DirectionalLight.h"
#include "DX11.h"

void DirectionalLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	const HRESULT result = DX11::myContext->Map
	(
		aLightBuffer.Get(),
		0, D3D11_MAP_WRITE_DISCARD,
		0,
		&bufferData
	);

	if (FAILED(result))
	{
		return;
	}

	memcpy_s(bufferData.pData, sizeof(LightBufferData), &ourlightBuffer, sizeof(LightBufferData));

	DX11::myContext->Unmap(aLightBuffer.Get(), 0);

	DX11::myContext->PSSetConstantBuffers
	(
		3,
		1,
		aLightBuffer.GetAddressOf()
	);

}

