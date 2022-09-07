#include "Core/GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <fstream>
#include "Renderer/Vertex.h"
#include "Renderer/Camera.h"
#include "Renderer/ModelInstance.h"
#include "CU/Timer.hpp"
#include "Renderer/LightAssetHandler.h"
#include "Renderer/TextureAssetHandler.h"
#include "Renderer/ParticleAssetHandler.h"
#include "Scene/SceneSerializer.h"
#include "Editor/SettingsPanel.h"

CommonUtilities::InputHandler GraphicsEngine::myInputHandler;
CommonUtilities::Timer GraphicsEngine::myTimer;

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY,
	unsigned someWidth, unsigned someHeight,
	bool enableDeviceDebug)
{
#ifdef DEBUG
	// Create Console Window
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
#endif // DEBUG

	if (!myInstance)
	{
		myInstance = this;
	}
	else
	{
		return false;
	}

	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = GraphicsEngine::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"TGP";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"TGP",
		L"Tillämpad Grafikprogrammering",
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		someX,
		someY,
		someWidth,
		someHeight,
		nullptr, nullptr, nullptr,
		this
		);

	// F1 -- This is where we should init our Framework
	if (!myFramework.Initialize(myWindowHandle, false))
	{
		return false;
	}
	
	if (!myForwardRenderer.Initialize())
	{
		return false;
	}
	
	HRESULT result = S_FALSE;

	D3D11_BLEND_DESC alphaBlendDesc = {};
	alphaBlendDesc.RenderTarget[0].BlendEnable = true;
	alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = DX11::myDevice->CreateBlendState(&alphaBlendDesc, &myBlendStates[BlendState::BS_AlphaBlend]);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BLEND_DESC additiveBlendDesc = {};
	additiveBlendDesc.RenderTarget[0].BlendEnable = true;
	additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = DX11::myDevice->CreateBlendState(&additiveBlendDesc, &myBlendStates[BlendState::BS_Additive]);
	if (FAILED(result))
	{
		return false;
	}

	myBlendStates[BlendState::BS_None] = nullptr;

	D3D11_DEPTH_STENCIL_DESC readOnlyDepthDesc = {};
	readOnlyDepthDesc.DepthEnable = true;
	readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	readOnlyDepthDesc.StencilEnable = false;

	result = DX11::myDevice->CreateDepthStencilState(&readOnlyDepthDesc, &myDepthStencilStates[DepthStencilState::DSS_ReadOnly]);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC offDepthDesc = {};
	offDepthDesc.DepthEnable = false;
	offDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	offDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	offDepthDesc.StencilEnable = false;

	result = DX11::myDevice->CreateDepthStencilState(&offDepthDesc, &myDepthStencilStates[DepthStencilState::DSS_Off]);
	if (FAILED(result))
	{
		return false;
	}

	myDepthStencilStates[DepthStencilState::DSS_ReadWrite] = nullptr;

	RECT clientRect;
	GetClientRect(GetWindowHandle(), &clientRect);

	myGBuffer = TextureAssetHandler::CreateGBuffer(
		clientRect.right - clientRect.left, 
		clientRect.bottom - clientRect.top);

	myDeferredRenderer.Init();
	myImGuiLayer.OnAttach(GetWindowHandle());

	if (!InitializeScene())
	{
		return false;
	}

	return true;
}

bool GraphicsEngine::InitializeScene()
{	
	myScene = CreateRef<Scene>();
	Scene::SetActiveScene(myScene);

	SceneSerializer serializer(myScene);
	serializer.Deserialize("../Assets/Scenes/default");
	serializer.DeserializeSettings("../Assets/Settings/default");
	serializer.DeserializePreset((std::string("../Assets/Settings/") + SettingsPanel::preset1).c_str());
	SettingsPanel::preset1Color = DX11::myClearColor;
	serializer.DeserializePreset((std::string("../Assets/Settings/") + SettingsPanel::preset2).c_str());
	SettingsPanel::preset2Color = DX11::myClearColor;
	
	SettingsPanel::colorSlider = SettingsPanel::preset1Color;

	myCamera = myScene->GetEntityFromUUID(7509847562195690);
	myScene->SetMainCamera(myCamera);

	myModelAssetHandler.Init();
	ParticleAssetHandler::Init();

	if (myCamera.IsValid())
	{
		auto& cameraComp = myCamera.GetComponent<CameraComponent>();
		cameraComp.camera.SetProjectionValues(90, 9.f / 16.f, 0.1f, 10000.0f);
	}

	auto cube = myScene->GetEntityFromUUID(6057646693852787664);
	if (cube.IsValid())
	{
		auto& cubeModelComp = cube.GetComponent<ModelComponent>();
		cubeModelComp.modelInstance = *myModelAssetHandler.GetModelInstance("Cube").get();
	}

	auto chest = myScene->GetEntityFromUUID(3316484318424843108);
	if (chest.IsValid())
	{
		auto& chestModelComp = chest.GetComponent<ModelComponent>();

		myModelAssetHandler.LoadModel("Models/SM/Particle_Chest.fbx");
		chestModelComp.modelInstance = *myModelAssetHandler.GetModelInstance("Models/SM/Particle_Chest.fbx").get();
	}

	auto gremlin = myScene->GetEntityFromUUID(3446174191707793529);
	if (gremlin.IsValid())
	{
		auto& gremlinModelComp = gremlin.GetComponent<ModelComponent>();

		myModelAssetHandler.LoadModel("Models/SK/gremlin.fbx");
		myModelAssetHandler.LoadAnimation("Models/SK/gremlin.fbx", "Models/Animations/gremlin@run.fbx");
		myModelAssetHandler.LoadAnimation("Models/SK/gremlin.fbx", "Models/Animations/gremlin@walk.fbx");
		gremlinModelComp.modelInstance = *myModelAssetHandler.GetModelInstance("Models/SK/gremlin.fbx").get();
		gremlinModelComp.modelInstance.SetAnimation("Models/Animations/gremlin@walk.fbx");
		gremlinModelComp.modelInstance.SetAnimationState(eAnimationState::Playing);
	}

	auto particleSystem = myScene->GetEntityFromUUID(3905982894324659577);
	if (particleSystem.IsValid())
	{
		auto& particleSystemComp = particleSystem.GetComponent<ParticleSystemComponent>();

		particleSystemComp.system = *ParticleAssetHandler::GetParticleSystem("Core").get();
	}

	TextureAssetHandler::LoadTexture("studio_cubemap.dds");

	myDirectionalLight = LightAssetHandler::CreateDirectionalLight({ 1.0f, 1.0f, 1.0f }, 0.5f, { 0.f, 0.f, 0.f });
	myEnvironmentLight = LightAssetHandler::CreateEnvironmentLight("studio_cubemap.dds");

	return true;
}

bool GraphicsEngine::CleanUp()
{
	myImGuiLayer.OnDetach();
	return false;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (::ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	myInputHandler.UpdateEvents(uMsg, wParam, lParam);
	
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if(uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	else if (uMsg == WM_CREATE)
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
	// F1 - This is where we clear our buffers and start the DX frame.
	SetBlendState(BlendState::BS_None);
	SetDepthStencilState(DepthStencilState::DSS_ReadWrite);
	myFramework.BeginFrame();
	myGBuffer->Clear();
	myImGuiLayer.Begin();
}

void GraphicsEngine::RenderFrame()
{
	myTimer.Update();

	// Will be fleshed out later!
	if (myScene)
	{
		Controller();
		myEditorLayer.OnRender();

		Entity camera = myScene->GetMainCamera();
		std::vector<Entity> modelEntitiesToRender = myScene->CullModels(camera);
		std::vector<Entity> lightEntitiesToRender = myScene->CullLights(camera);
		std::vector<Entity> particlesEntitiesToRender = myScene->CullParticles(camera);
		
		//myGBuffer->SetAsTarget();
		//myDeferredRenderer.GenereteGBuffer(camera, modelEntitiesToRender, myTimer.GetDeltaTime(), myTimer.GetTotalTime());
		//myGBuffer->ClearTarget();
		//myGBuffer->SetAsResource(0);
		//DX11::myContext->OMSetRenderTargets(1, DX11::myRenderTarget.GetAddressOf(), DX11::myDepthStencil.Get());
		//SetDepthStencilState(DepthStencilState::DSS_Off);
		//myDeferredRenderer.Render(camera, myDirectionalLight, myEnvironmentLight, myTimer.GetDeltaTime(), myTimer.GetTotalTime());

		myForwardRenderer.RenderModels(camera, modelEntitiesToRender, lightEntitiesToRender, myDirectionalLight, myEnvironmentLight);

		//SetBlendState(BlendState::BS_Additive);
		//SetDepthStencilState(DepthStencilState::DSS_ReadOnly);
		//myForwardRenderer.RenderParticles(camera, particlesEntitiesToRender);
	}
}

void GraphicsEngine::EndFrame()
{
	// F1 - This is where we finish our rendering and tell the framework
	// to present our result to the screen.
	myImGuiLayer.End();
	myFramework.EndFrame();
}

void GraphicsEngine::Controller()
{
	if (!movementActive) { return; }

	static float moveSpeed = 100.f;
	static float mouseSens = 0.25f;

	auto& transform = myCamera.GetComponent<TransformComponent>();

	if (myInputHandler.IsKeyDown(KeyCode::W))
	{
		transform.position += (Matrix4::Forward(transform.GetTransform()) * moveSpeed * myTimer.GetDeltaTime());
	}
	if (myInputHandler.IsKeyDown(KeyCode::A))
	{
		transform.position += (Matrix4::Right(transform.GetTransform()) * -moveSpeed * myTimer.GetDeltaTime());
	}
	if (myInputHandler.IsKeyDown(KeyCode::S))
	{
		transform.position += (Matrix4::Forward(transform.GetTransform()) * -moveSpeed * myTimer.GetDeltaTime());
	}
	if (myInputHandler.IsKeyDown(KeyCode::D))
	{
		transform.position += (Matrix4::Right(transform.GetTransform()) * moveSpeed * myTimer.GetDeltaTime());
	}

	if (myInputHandler.IsKeyDown(KeyCode::E))
	{
		transform.position.y += (moveSpeed * myTimer.GetDeltaTime());
	}
	if (myInputHandler.IsKeyDown(KeyCode::Q))
	{
		transform.position.y += (-moveSpeed * myTimer.GetDeltaTime());
	}

	static POINT prevFrame = myInputHandler.GetMousePosition();
	static POINT currFrame = myInputHandler.GetMousePosition();
	if (myInputHandler.IsKeyDown(KeyCode::MOUSERBUTTON))
	{
		currFrame = myInputHandler.GetMousePosition();
		auto deltaX = currFrame.x - prevFrame.x;
		auto deltaY = currFrame.y - prevFrame.y;
		if (deltaX != 0 || deltaY != 0)
		{
			transform.rotation += Vector3f(
				deltaY * mouseSens * myTimer.GetDeltaTime(),
				deltaX * mouseSens * myTimer.GetDeltaTime(), 
				0.f);
		}
	}
	prevFrame = myInputHandler.GetMousePosition();
}

void GraphicsEngine::SetBlendState(BlendState aState)
{
	DX11::myContext->OMSetBlendState(myBlendStates[aState].Get(), nullptr, 0xffffffff);
}

void GraphicsEngine::SetDepthStencilState(DepthStencilState aState)
{
	DX11::myContext->OMSetDepthStencilState(myDepthStencilStates[aState].Get(), 0);
}