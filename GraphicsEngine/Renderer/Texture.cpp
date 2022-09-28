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
	DX11::myContext->ClearRenderTargetView(myRTV.Get(), &DX11::myClearColor[0]);
}
