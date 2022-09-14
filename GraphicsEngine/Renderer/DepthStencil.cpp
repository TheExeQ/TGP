#include "DepthStencil.h"
#include "Core/DX11.h"

void DepthStencil::SetAsDepth()
{
	DX11::myContext->OMSetRenderTargets(1, DX11::myRenderTarget.GetAddressOf(), myDSV.Get());
}

void DepthStencil::Clear()
{
	DX11::myContext->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
