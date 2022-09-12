#include "Core/DX11.h"
#include "TextureAssetHandler.h"
#include "DDSTextureLoader11.h"

#include <cassert>

std::unordered_map<std::string, Ref<Texture>> TextureAssetHandler::myRegistry;

Ref<Texture> TextureAssetHandler::GetTexture(const std::string& aTextureName)
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
			myRegistry.insert({ aFileName, CreateRef<Texture>(result) });
			return true;
		}
	}
	return false;
}

Scope<GBuffer> TextureAssetHandler::CreateGBuffer(int aWidth, int aHeight)
{
	GBuffer gbuffer;
	for (int i = 0; i < GBuffer::GBufferTexture::GB_COUNT; i++)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> bufferTexture;
		D3D11_TEXTURE2D_DESC bufferDesc = {};
		bufferDesc.Width = aWidth;
		bufferDesc.Height = aHeight;
		bufferDesc.ArraySize = 1;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		switch (i)
		{
		case GBuffer::GBufferTexture::GB_ALBEDO:
		{
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_NORMAL:
		{
			bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_MATERIAL:
		{
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_VERTEXNORMAL:
		{
			bufferDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		}
		case GBuffer::GBufferTexture::GB_POSITION:
		{
			bufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		case GBuffer::GBufferTexture::GB_AMBIENTOCCLUSION:
		{
			bufferDesc.Format = DXGI_FORMAT_R8_UNORM;
			break;
		}
		default:
		{
			break;
		}
		}

		DX11::myDevice->CreateTexture2D(&bufferDesc, nullptr, bufferTexture.GetAddressOf());

		DX11::myDevice->CreateRenderTargetView(bufferTexture.Get(), nullptr, gbuffer.myRTVs[i].GetAddressOf());
		DX11::myDevice->CreateShaderResourceView(bufferTexture.Get(), nullptr, gbuffer.mySRVs[i].GetAddressOf());
	}
	return CreateScope<GBuffer>(gbuffer);
}

Scope<DepthStencil> TextureAssetHandler::CreateDepthStencil(const std::string aName, size_t aWidth, size_t aHeight)
{
	HRESULT result;

	Scope<DepthStencil> output = CreateScope<DepthStencil>();
	output->myName = aName;

	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = static_cast<unsigned int>(aWidth);
	desc.Height = static_cast<unsigned int>(aHeight);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	result = DX11::myDevice->CreateTexture2D(&desc, nullptr, reinterpret_cast<ID3D11Texture2D**>(output->myTexture.GetAddressOf()));

	assert(SUCCEEDED(result));

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = desc.MipLevels;
	result = DX11::myDevice->CreateShaderResourceView(output->myTexture.Get(), &srv_desc, output->mySRV.GetAddressOf());

	assert(SUCCEEDED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	result = DX11::myDevice->CreateDepthStencilView(output->myTexture.Get(), &dsv_desc, output->myDSV.GetAddressOf());

	assert(SUCCEEDED(result));

	output->myViewport = D3D11_VIEWPORT({0.f, 0.f, static_cast<float>(aWidth), static_cast<float>(aHeight), 0.0f, 1.0f});

	return output;
}

