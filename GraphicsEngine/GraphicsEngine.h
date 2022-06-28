#pragma once
#include <vector>
#include <memory>
#include "DX11.h"
#include "ForwardRenderer.h"
#include "Scene.h"
#include "ModelAssetHandler.h"
#include "CU/Timer.hpp"
#include "CU/InputHandler.hpp"
#include "DirectionalLight.h"
#include "EnvironmentLight.h"

enum BlendState
{
	BS_None,
	BS_AlphaBlend,
	BS_Additive,
	BS_Count,
};

enum DepthStencilState
{
	DSS_ReadWrite,
	DSS_ReadOnly,
	DSS_Count,
};

class GraphicsEngine
{
	SIZE myWindowSize{0,0};
	HWND myWindowHandle{};

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

public:

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug);
	bool InitializeScene();

	void BeginFrame();
	void EndFrame();
	void RenderFrame();

	static const CommonUtilities::Timer& GetTimer() { return myTimer; };

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }

	void SetBlendState(BlendState aState);
	void SetDepthStencilState(DepthStencilState aState);

private:
	void Controller();
	
	DX11 myFramework;
	ForwardRenderer myForwardRenderer;
	std::shared_ptr<Scene> myScene;
	ModelAssetHandler myModelAssetHandler;
	
	std::shared_ptr<DirectionalLight> myDirectionalLight;
	std::shared_ptr<EnvironmentLight> myEnvironmentLight;

	std::array<ComPtr<ID3D11BlendState>, BlendState::BS_Count> myBlendStates;
	std::array<ComPtr<ID3D11DepthStencilState>, DepthStencilState::DSS_Count> myDepthStencilStates;

	static CommonUtilities::InputHandler myInputHandler;
	static CommonUtilities::Timer myTimer;
};

