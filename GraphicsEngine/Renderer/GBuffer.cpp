#include "GBuffer.h"

void GBuffer::SetAsTarget() const
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::GB_COUNT];
	for (int i = 0; i < myRTVs.size(); i++)
	{
		rtvList[i] = myRTVs[i].Get();
	}

	DX11::myContext->OMSetRenderTargets(GBufferTexture::GB_COUNT, &rtvList[0], DX11::myDepthStencil.Get());
}

void GBuffer::ClearTarget() const
{
	ID3D11RenderTargetView* rtvList[GBufferTexture::GB_COUNT];
	for (int i = 0; i < myRTVs.size(); i++)
	{
		rtvList[i] = nullptr;
	}

	DX11::myContext->OMSetRenderTargets(GBufferTexture::GB_COUNT, &rtvList[0], DX11::myDepthStencil.Get());
}

void GBuffer::SetAsResource(unsigned int aStartSlot) const
{
	ID3D11ShaderResourceView* srvList[GBufferTexture::GB_COUNT];
	for (uint16_t t = 0; t < mySRVs.size(); t++)
	{
		srvList[t] = mySRVs[t].Get();
	}

	DX11::myContext->PSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &srvList[0]);
}

void GBuffer::ClearResource(unsigned int aStartSlot) const
{
	ID3D11ShaderResourceView* srvList[GBufferTexture::GB_COUNT];
	for (uint16_t t = 0; t < mySRVs.size(); t++)
	{
		srvList[t] = nullptr;
	}

	DX11::myContext->PSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &srvList[0]);
}

void GBuffer::Clear() const
{
	std::array<float, 4> color{ 0.0f, 0.0f, 0.0f, 0.0f };
	for (uint8_t t = 0; t < myRTVs.size(); t++)
	{
		DX11::myContext->ClearRenderTargetView(myRTVs[t].Get(), &color[0]);
	}
}
