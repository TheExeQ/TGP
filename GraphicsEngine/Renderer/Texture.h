#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

using namespace Microsoft::WRL;

class Texture
{
public:
	virtual ~Texture();

	void SetAsTarget();
	void SetAsResource(uint32_t aSlot);
	void ClearRTV();

	size_t GetWidth() const { return myWidth; };
	size_t GetHeight() const { return myHeight; };

protected:
	std::string myName;
	
	ComPtr<ID3D11Resource> myTexture;
	ComPtr<ID3D11RenderTargetView> myRTV;
	ComPtr<ID3D11ShaderResourceView> mySRV;

	size_t myWidth, myHeight;
private:
	friend class TextureAssetHandler;
};