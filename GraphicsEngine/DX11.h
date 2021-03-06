#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class DX11
{
public:
	static bool Initialize(HWND aWindowHandle, bool aDebug);
	static void BeginFrame();
	static void EndFrame();

	static ComPtr<ID3D11Device> myDevice;
	static ComPtr<ID3D11DeviceContext> myContext;
	static ComPtr<IDXGISwapChain> mySwapchain;
	static ComPtr<ID3D11RenderTargetView> myRenderTarget;
	static ComPtr<ID3D11DepthStencilView> myDepthStencil;
};

