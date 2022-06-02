#include "Texture.h"
#include "DX11.h"

Texture::~Texture()
{
	myTexture.Reset();
	mySRV.Reset();
}

void Texture::SetAsResource(uint32_t aSlot)
{
	DX11::myContext->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}
