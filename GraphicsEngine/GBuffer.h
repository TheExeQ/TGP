#pragma once
#include "DX11.h"
#include <array>

class GBuffer
{
public:
	enum GBufferTexture
	{
		GB_ALBEDO,
		GB_NORMAL,
		GB_MATERIAL,
		GB_VERTEXNORMAL,
		GB_POSITION,
		GB_AMBIENTOCCLUSION,
		GB_COUNT
	};

public:
	void SetAsTarget() const;
	void ClearTarget() const;
	void SetAsResource(unsigned int aStartSlot) const;
	void ClearResource(unsigned int aStartSlot) const;
	void Clear() const;

private:
	friend class TextureAssetHandler;

	std::array<ComPtr<ID3D11RenderTargetView>, GBufferTexture::GB_COUNT> myRTVs;
	std::array<ComPtr<ID3D11ShaderResourceView>, GBufferTexture::GB_COUNT> mySRVs;
};