#include "gepch.h"
#include "Texture.h"
#include "Core/DX11.h"

Texture::~Texture()
{
	myTexture.Reset();
	mySRV.Reset();
}

void Texture::SetAsTarget()
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = myWidth;
	viewport.Height = myHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	DX11::myContext->RSSetViewports(1, &viewport);
	DX11::myContext->OMSetRenderTargets(1, myRTV.GetAddressOf(), nullptr);
}

void Texture::SetAsResource(uint32_t aSlot)
{
	DX11::myContext->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}

void Texture::ClearRTV()
{
	std::array<float, 4> clearColor = {0.f, 0.f, 0.f, 0.f};
	DX11::myContext->ClearRenderTargetView(myRTV.Get(), &DX11::myClearColor[0]);
}
