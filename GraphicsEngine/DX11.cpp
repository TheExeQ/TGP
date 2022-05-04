#include "GraphicsEngine.pch.h"
#include "DX11.h"

ComPtr<ID3D11Device> DX11::myDevice = nullptr;
ComPtr<ID3D11DeviceContext> DX11::myContext = nullptr;
ComPtr<IDXGISwapChain> DX11::mySwapchain = nullptr;
ComPtr<ID3D11RenderTargetView> DX11::myRenderTarget = nullptr;
ComPtr<ID3D11DepthStencilView> DX11::myDepthStencil = nullptr;

bool DX11::Initialize(HWND aWindowHandle, bool aDebug)
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = aWindowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

	result = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		aDebug ? D3D11_CREATE_DEVICE_DEBUG : 0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapchain,
		&myDevice,
		nullptr,
		&myContext
	);
	if (FAILED(result))
	{
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferTexture;
	result = mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBufferTexture);
	if (FAILED(result))
	{
		return false;
	}

	result = myDevice->CreateRenderTargetView(backBufferTexture.Get(), nullptr, myRenderTarget.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	result = backBufferTexture->Release();
	if (FAILED(result))
	{
		return false;
	}

	RECT clientRect = { 0, 0, 0, 0 };
	GetClientRect(aWindowHandle, &clientRect);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = clientRect.right - clientRect.left;
	depthBufferDesc.Height = clientRect.bottom - clientRect.top;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	result = myDevice->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	result = myDevice->CreateDepthStencilView(depthBufferTexture.Get(), nullptr, myDepthStencil.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	
	result = depthBufferTexture->Release();
	if (FAILED(result))
	{
		return false;
	}

	myContext->OMSetRenderTargets(1, myRenderTarget.GetAddressOf(), myDepthStencil.Get());
	if (FAILED(result))
	{
		return false;
	}

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(clientRect.right - clientRect.left);
	viewport.Height = static_cast<float>(clientRect.bottom - clientRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	myContext->RSSetViewports(1, &viewport);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

void DX11::BeginFrame()
{
	float color[] = { 0.f, 1.f, 1.0f, 1.f };
	myContext->ClearRenderTargetView(myRenderTarget.Get(), color);
	myContext->ClearDepthStencilView(myDepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11::EndFrame()
{
	mySwapchain->Present(1, 0); 
}
