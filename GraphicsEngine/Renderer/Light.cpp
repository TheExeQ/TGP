#include "Light.h"
#include "Core/DX11.h"

void Light::Init(Vector3<float> aColor, float anIntensity)
{
	ourLightBuffer.Color = aColor;
	ourLightBuffer.Intensity = anIntensity;
}

void Light::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
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

	memcpy_s(bufferData.pData, sizeof(LightBufferData), &ourLightBuffer, sizeof(LightBufferData));

	DX11::myContext->Unmap(aLightBuffer.Get(), 0);

	DX11::myContext->PSSetConstantBuffers
	(
		3,
		1,
		aLightBuffer.GetAddressOf()
	);
}

void Light::SetShadowMapAsDepth(const int& aIndex)
{
	if (aIndex > myShadowMaps.size() - 1) { return; }
	myShadowMaps[aIndex]->SetAsDepth();
}

void Light::SetShadowMapAsResource(const int& aSlot, const int& aAmount)
{
	if (aAmount > 6) { return; }
	for (int i = 0; i < aAmount; i++)
	{
		myShadowMaps[i]->SetAsResource(aSlot + i);
	}
}

void Light::SetLightView(const Matrix4& lightView)
{
	ourLightBuffer.LightView = Matrix4::GetFastInverse(lightView);
}

