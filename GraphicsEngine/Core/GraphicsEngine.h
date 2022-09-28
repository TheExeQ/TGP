#pragma once
#include <vector>
#include <memory>
#include "Core/DX11.h"
#include "Renderer/ForwardRenderer.h"
#include "Renderer/DeferredRenderer.h"
#include "Renderer/ShadowRenderer.h"
#include "Renderer/PostProcessRenderer.h"
#include "Renderer/GBuffer.h"
#include "Scene/Scene.h"
#include "Renderer/ModelAssetHandler.h"
#include "CU/Timer.hpp"
#include "CU/InputHandler.hpp"
#include "Renderer/DirectionalLight.h"
#include "Renderer/EnvironmentLight.h"
#include "ImGui/ImGuiLayer.h"
#include "Scene/Entity.h"

#include "Editor/EditorLayer.h"

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
	DSS_Off,
	DSS_Count,
};

enum SamplerState
{
	SS_Default,
	SS_PointClamp,
	SS_Count,
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
	bool CleanUp();

	void BeginFrame();
	void EndFrame();
	void RenderFrame();

	static const CommonUtilities::Timer& GetTimer() { return myTimer; };

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }

	void SetBlendState(BlendState aState);
	void SetDepthStencilState(DepthStencilState aState);
	void SetSamplerState(SamplerState aState, uint32_t aSlot);

	void ResetStates();

	Scope<GBuffer>& GetGBuffer() { return myGBuffer; };

	static GraphicsEngine& Get() { return *myInstance; };

private:
	friend class SceneHierarchyPanel;
	friend class SettingsPanel;
	void Controller();
	
	Entity myCamera;

	DX11 myFramework;
	ForwardRenderer myForwardRenderer;
	DeferredRenderer myDeferredRenderer;
	ShadowRenderer myShadowRenderer;
	PostProcessRenderer myPostProcessRenderer;
	Ref<Scene> myScene;
	ModelAssetHandler myModelAssetHandler;
	
	Scope<GBuffer> myGBuffer;
	Ref<DirectionalLight> myDirectionalLight;
	Ref<EnvironmentLight> myEnvironmentLight;

	std::array<ComPtr<ID3D11BlendState>, BlendState::BS_Count> myBlendStates;
	std::array<ComPtr<ID3D11DepthStencilState>, DepthStencilState::DSS_Count> myDepthStencilStates;
	std::array<ComPtr<ID3D11SamplerState>, SamplerState::SS_Count> mySamplerStates;

	static CommonUtilities::InputHandler myInputHandler;
	static CommonUtilities::Timer myTimer;
	EditorLayer myEditorLayer;
	ImGuiLayer myImGuiLayer;

	inline static bool movementActive = true;

	Ref<Texture> myIntermediateTargetA;
	Ref<Texture> myIntermediateTargetB;
	Ref<Texture> myHalfSizeTarget;
	Ref<Texture> myQuarterSizeTarget;
	Ref<Texture> myBlurTargetA;
	Ref<Texture> myBlurTargetB;

	inline static GraphicsEngine* myInstance = nullptr;
};

