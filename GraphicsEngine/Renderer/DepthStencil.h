#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Texture.h"

using namespace Microsoft::WRL;

class DepthStencil : public Texture
{
public:
	~DepthStencil() 
	{
		myDSV.Reset();
	}

	void SetAsDepth();
	void Clear();

protected:
	ComPtr<ID3D11DepthStencilView> myDSV;
	D3D11_VIEWPORT myViewport = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
private:
	friend class TextureAssetHandler;
};