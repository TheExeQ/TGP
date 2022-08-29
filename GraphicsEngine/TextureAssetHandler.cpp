#include "DX11.h"
#include "TextureAssetHandler.h"
#include "DDSTextureLoader11.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> TextureAssetHandler::myRegistry;

std::shared_ptr<Texture> TextureAssetHandler::GetTexture(const std::string& aTextureName)
{
	return myRegistry[aTextureName];
}

bool TextureAssetHandler::LoadTexture(const std::string& aFileName)
{
	if (const auto it = myRegistry.find(aFileName); it == myRegistry.end())
	{
		std::wstring fileName = L"../Assets/Textures/" + std::wstring(aFileName.begin(), aFileName.end());
		Texture result;
		const HRESULT hrResult = DirectX::CreateDDSTextureFromFile(DX11::myDevice.Get(), fileName.c_str(), result.myTexture.GetAddressOf(), result.mySRV.GetAddressOf());
		
		if (FAILED(hrResult))
		{
			return false;
		}
		else
		{
			myRegistry.insert({ aFileName, std::make_shared<Texture>(result) });
			return true;
		}
	}
	return false;
}

std::unique_ptr<GBuffer> TextureAssetHandler::CreateGBuffer(int aWidth, int aHeight)
{

	return std::unique_ptr<GBuffer>();
}

