#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <array>

using namespace Microsoft::WRL;

class DX11
{
public:
	static bool Initialize(HWND aWindowHandle, bool aDebug);
	static void BeginFrame();
	static void EndFrame();

	static void SetViewport(const int& width, const int& height);

	static ComPtr<ID3D11Device> myDevice;
	static ComPtr<ID3D11DeviceContext> myContext;
	static ComPtr<IDXGISwapChain> mySwapchain;
	static ComPtr<ID3D11RenderTargetView> myRenderTarget;
	static ComPtr<ID3D11DepthStencilView> myDepthStencil;

	inline static RECT myClientRect = { 0, 0, 0, 0 };
	inline static std::array<float, 4> myClearColor;
};

