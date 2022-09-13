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

protected:
	ComPtr<ID3D11DepthStencilView> myDSV;
	D3D11_VIEWPORT myViewport;
private:
	friend class TextureAssetHandler;
};