#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

using namespace Microsoft::WRL;

class Texture
{
public:
	virtual ~Texture();

	void SetAsResource(uint32_t aSlot);

protected:
	std::string myName;
	
	ComPtr<ID3D11Resource> myTexture;
	ComPtr<ID3D11ShaderResourceView> mySRV;
private:
	friend class TextureAssetHandler;
};