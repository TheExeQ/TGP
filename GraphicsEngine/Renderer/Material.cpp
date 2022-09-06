#include "Material.h"
#include "Core/DX11.h"

Material::Material()
{

}

void Material::Init(std::string aName, const CommonUtilities::Vector3<float>& anAlbedo)
{
	myName = aName;
	myMaterialData.Albedo = anAlbedo;
}

void Material::SetAlbedoTexture(Ref<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Albedo] = aTexture;
}

void Material::SetNormalTexture(Ref<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Normal] = aTexture;
}

void Material::SetMaterialTexture(Ref<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Material] = aTexture;
}

void Material::SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer) const
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(bufferData));
	const HRESULT result = DX11::myContext->Map(
	aMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	
	if (FAILED(result))
	{
		return;
	}
	
	memcpy_s(bufferData.pData, sizeof(MaterialData), &myMaterialData, sizeof(MaterialData));

	DX11::myContext->Unmap(aMaterialBuffer.Get(), 0);

	if (myTextures[MaterialTextureChannel::Albedo])
	{
		myTextures[MaterialTextureChannel::Albedo]->SetAsResource(0);
	}

	if (myTextures[MaterialTextureChannel::Normal])
	{
		myTextures[MaterialTextureChannel::Normal]->SetAsResource(1);
	}

	if (myTextures[MaterialTextureChannel::Material])
	{
		myTextures[MaterialTextureChannel::Material]->SetAsResource(2);
	}
}