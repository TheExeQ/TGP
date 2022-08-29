#include "GBuffer.h"

void GBuffer::SetAsTarget() const
{

}

void GBuffer::ClearTarget() const
{

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

}

void GBuffer::Clear() const
{

}
