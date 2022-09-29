#include "Texture.h"
#include "Core/DX11.h"

Texture::~Texture()
{
	myTexture.Reset();
	mySRV.Reset();
}

void Texture::SetAsTarget()
{
	DX11::myContext->OMSetRenderTargets(1, myRTV.GetAddressOf(), nullptr);
}

void Texture::SetAsResource(uint32_t aSlot)
{
	DX11::myContext->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}

void Texture::ClearRTV()
{
	std::array<float, 4> clearColor = {0.f, 0.f, 0.f, 0.f};
	DX11::myContext->ClearRenderTargetView(myRTV.Get(), &clearColor[0]);
}
