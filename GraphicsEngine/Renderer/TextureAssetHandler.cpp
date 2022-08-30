#include "Core/DX11.h"
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
	GBuffer gbuffer;
	for (int i = 0; i < GBuffer::GBufferTexture::GB_COUNT; i++)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> rtvBufferTexture;
		D3D11_TEXTURE2D_DESC rtvBufferDesc = {};
		rtvBufferDesc.Width = aWidth;
		rtvBufferDesc.Height = aHeight;
		rtvBufferDesc.ArraySize = 1;
		rtvBufferDesc.SampleDesc.Count = 1;
		rtvBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> srvBufferTexture;
		D3D11_TEXTURE2D_DESC srvBufferDesc = {};
		srvBufferDesc.Width = aWidth;
		srvBufferDesc.Height = aHeight;
		srvBufferDesc.ArraySize = 1;
		srvBufferDesc.SampleDesc.Count = 1;
		srvBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		switch (i)
		{
		case GBuffer::GBufferTexture::GB_ALBEDO:
		{
			rtvBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_NORMAL:
		{
			rtvBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
			srvBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_MATERIAL:
		{
			rtvBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_VERTEXNORMAL:
		{
			rtvBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
			srvBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_POSITION:
		{
			rtvBufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			srvBufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		case GBuffer::GBufferTexture::GB_AMBIENTOCCLUSION:
		{
			rtvBufferDesc.Format = DXGI_FORMAT_R8_UNORM;
			srvBufferDesc.Format = DXGI_FORMAT_R8_UNORM;
			break;
		}
		default:
		{
			break;
		}
		}

		DX11::myDevice->CreateTexture2D(&rtvBufferDesc, nullptr, rtvBufferTexture.GetAddressOf());
		DX11::myDevice->CreateTexture2D(&srvBufferDesc, nullptr, srvBufferTexture.GetAddressOf());

		DX11::myDevice->CreateRenderTargetView(rtvBufferTexture.Get(), nullptr, gbuffer.myRTVs[i].GetAddressOf());
		DX11::myDevice->CreateShaderResourceView(srvBufferTexture.Get(), nullptr, gbuffer.mySRVs[i].GetAddressOf());
	}
	return std::make_unique<GBuffer>(gbuffer);
}

