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
#include "Commands/CommandStack.h"

#include <filesystem>
#include <shellapi.h>

#define MAX_FILENAME 2000

CommonUtilities::InputHandler GraphicsEngine::myInputHandler;
CommonUtilities::Timer GraphicsEngine::myTimer;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (::ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	myInputHandler.UpdateEvents(uMsg, wParam, lParam);

	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	switch (uMsg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}

	case WM_CREATE:
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
		break;
	}

	case WM_DROPFILES:
	{
		HDROP droppedFile = (HDROP)wParam;

		TCHAR filePath[MAX_FILENAME];

		auto fileCount = DragQueryFile(droppedFile, -1, NULL, 0);

		for (int fileId = 0; fileId < fileCount; fileId++)
		{
			if (DragQueryFile(droppedFile, fileId, filePath, MAX_FILENAME) > 0)
			{
				int filePathLength = 0;

				for (int i = 0; i < MAX_FILENAME; i++)
				{
					if (filePath[i] == '\0')
					{
						filePathLength = i;
						break;
					}
				}

				int filePathNameBegin = 0;

				for (int i = 0; i < filePathLength; i++)
				{
					if (filePath[filePathLength - i] == '\\')
					{
						filePathNameBegin = filePathLength - i + 1;
						break;
					}
				}

				std::wstring fileName = L"../Assets/";
				std::filesystem::path somePath(filePath);

				auto ext = somePath.extension();

				if (ext.string() == ".fbx")
				{
					fileName.append(L"Models/");
				}
				else if (ext.string() == ".dds")
				{
					fileName.append(L"Textures/");
				}
				else if (ext.string() == ".scene")
				{
					fileName.append(L"Scenes/");
				}
				else if (ext.string() == ".settings" || ext.string() == ".preset")
				{
					fileName.append(L"Settings/");
				}

				for (int i = filePathNameBegin; i < filePathLength; i++)
				{
					fileName.push_back(filePath[i]);
				}

				auto orgFilename = fileName;
				orgFilename = orgFilename.substr(0, orgFilename.length() - ext.wstring().length());

				int count = 1;

				while (!CopyFile(filePath, fileName.c_str(), true))
				{
					fileName = orgFilename + std::to_wstring(count) + ext.wstring();
					count++;
				}
			}
		}

		DragFinish(droppedFile);
		break;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

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

	DragAcceptFiles(myWindowHandle, true);

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

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1.f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.BorderColor[0] = 0.f;
	samplerDesc.BorderColor[1] = 0.f;
	samplerDesc.BorderColor[2] = 0.f;
	samplerDesc.BorderColor[3] = 0.f;
	samplerDesc.MinLOD = 0.f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = DX11::myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[SamplerState::SS_Default]);

	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	result = DX11::myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[SamplerState::SS_Wrap]);

	if (FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC pointSamplerDesc;

	pointSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	pointSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSamplerDesc.MipLODBias = 0.f;
	pointSamplerDesc.MaxAnisotropy = 1.f;
	pointSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	pointSamplerDesc.BorderColor[0] = 0.f;
	pointSamplerDesc.BorderColor[1] = 0.f;
	pointSamplerDesc.BorderColor[2] = 0.f;
	pointSamplerDesc.BorderColor[3] = 0.f;
	pointSamplerDesc.MinLOD = -FLT_MAX;
	pointSamplerDesc.MaxLOD = FLT_MAX;

	result = DX11::myDevice->CreateSamplerState(&pointSamplerDesc, &mySamplerStates[SamplerState::SS_PointClamp]);

	if (FAILED(result))
	{
		return false;
	}

	pointSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	pointSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	pointSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	result = DX11::myDevice->CreateSamplerState(&pointSamplerDesc, &mySamplerStates[SamplerState::SS_PointWrap]);

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
	myShadowRenderer.Init();
	myPostProcessRenderer.Init();
	myImGuiLayer.OnAttach(GetWindowHandle());

	if (!InitializeScene())
	{
		return false;
	}

	RECT aRect = DX11::myClientRect;
	size_t width = aRect.right - aRect.left;
	size_t height = aRect.bottom - aRect.top;

	myIntermediateTargetA = TextureAssetHandler::CreateRenderTarget("IntermediateA", width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myIntermediateTargetB = TextureAssetHandler::CreateRenderTarget("IntermediateB", width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myHalfSizeTarget = TextureAssetHandler::CreateRenderTarget("HalfSize", width / 2, height / 2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myQuarterSizeTarget = TextureAssetHandler::CreateRenderTarget("QuarterSize", width / 4, height / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myBlurTargetA = TextureAssetHandler::CreateRenderTarget("BlurA", width / 4, height / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myBlurTargetB = TextureAssetHandler::CreateRenderTarget("BlurB", width / 4, height / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	mySSAOTarget = TextureAssetHandler::CreateRenderTarget("SSAOTarget", width, height, DXGI_FORMAT_R32_FLOAT);
	
	TextureAssetHandler::LoadTexture("BlueNoise.dds");
	myNoiceTexture = TextureAssetHandler::GetTexture("BlueNoise.dds");

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

	myDirectionalLight = LightAssetHandler::CreateDirectionalLight({ 1.0f, 1.0f, 1.0f }, 0.5f, { 0.f, 500.f, 0.f }, { 45.f, -45.f, 0 });
	myEnvironmentLight = LightAssetHandler::CreateEnvironmentLight("studio_cubemap.dds");

	return true;
}

bool GraphicsEngine::CleanUp()
{
	myImGuiLayer.OnDetach();
	return false;
}

void GraphicsEngine::BeginFrame()
{
	// F1 - This is where we clear our buffers and start the DX frame.
	ResetStates();
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

		DX11::myContext->OMSetRenderTargets(0, nullptr, nullptr);

		DX11::SetViewport(2048, 2048);

		myDirectionalLight->ClearShadowMap();
		myDirectionalLight->SetShadowMapAsDepth(0);
		myShadowRenderer.Render(lightEntitiesToRender, myDirectionalLight, modelEntitiesToRender);

		auto width = DX11::myClientRect.right - DX11::myClientRect.left;
		auto height = DX11::myClientRect.bottom - DX11::myClientRect.top;

		DX11::SetViewport(width, height);

		myGBuffer->SetAsTarget();
		myDeferredRenderer.GenereteGBuffer(camera, modelEntitiesToRender, myTimer.GetDeltaTime(), myTimer.GetTotalTime());
		myGBuffer->ClearTarget();
		myGBuffer->SetAsResource(0);

		mySSAOTarget->SetAsTarget();
		myNoiceTexture->SetAsResource(8);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_SSAO, mySSAOTarget->GetWidth(), mySSAOTarget->GetHeight(), camera);

		DX11::myContext->OMSetRenderTargets(1, DX11::myRenderTarget.GetAddressOf(), DX11::myDepthStencil.Get());
		SetDepthStencilState(DepthStencilState::DSS_Off);
		myDirectionalLight->SetShadowMapAsResource(6, 1);

		bool foundSpot = false, foundPoint = false;

		for (auto light : lightEntitiesToRender)
		{
			auto data = light.GetComponent<LightComponent>().light;
			switch (data.GetLightBufferData().LightType)
			{
			case 1:
			{
				data.SetShadowMapAsResource(8, 6);
				foundPoint = true;
				break;
			}
			case 2:
			{
				data.SetShadowMapAsResource(7, 1);
				foundSpot = true;
				break;
			}
			default:
			{

				break;
			}
			}
		}

		myIntermediateTargetA->SetAsTarget();
		myDeferredRenderer.Render(camera, lightEntitiesToRender, myDirectionalLight, myEnvironmentLight, myTimer.GetDeltaTime(), myTimer.GetTotalTime());

		//myForwardRenderer.RenderModels(camera, modelEntitiesToRender, lightEntitiesToRender, myDirectionalLight, myEnvironmentLight);

		SetBlendState(BlendState::BS_Additive);
		SetDepthStencilState(DepthStencilState::DSS_Off);
		myForwardRenderer.RenderParticles(camera, particlesEntitiesToRender);

		SetBlendState(BlendState::BS_None);
		SetDepthStencilState(DepthStencilState::DSS_ReadWrite);

		myIntermediateTargetB->SetAsTarget();
		myIntermediateTargetA->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_LUMINANCE, myIntermediateTargetB->GetWidth(), myIntermediateTargetB->GetHeight());

		myHalfSizeTarget->SetAsTarget();
		myIntermediateTargetB->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_COPY, myHalfSizeTarget->GetWidth(), myHalfSizeTarget->GetHeight());

		myQuarterSizeTarget->SetAsTarget();
		myHalfSizeTarget->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_COPY, myQuarterSizeTarget->GetWidth(), myQuarterSizeTarget->GetHeight());

		myBlurTargetA->SetAsTarget();
		myQuarterSizeTarget->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_GAUSSIAN, myBlurTargetA->GetWidth(), myBlurTargetA->GetHeight());

		myBlurTargetB->SetAsTarget();
		myBlurTargetA->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_GAUSSIAN, myBlurTargetB->GetWidth(), myBlurTargetB->GetHeight());

		myQuarterSizeTarget->SetAsTarget();
		myBlurTargetB->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_COPY, myQuarterSizeTarget->GetWidth(), myQuarterSizeTarget->GetHeight());

		myHalfSizeTarget->SetAsTarget();
		myQuarterSizeTarget->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_COPY, myHalfSizeTarget->GetWidth(), myHalfSizeTarget->GetHeight());

		D3D11_VIEWPORT viewport = {};
		viewport.Width = DX11::myClientRect.right - DX11::myClientRect.left;
		viewport.Height = DX11::myClientRect.bottom - DX11::myClientRect.top;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		DX11::myContext->RSSetViewports(1, &viewport);
		DX11::myContext->OMSetRenderTargets(1, DX11::myRenderTarget.GetAddressOf(), DX11::myDepthStencil.Get());
		myIntermediateTargetA->SetAsResource(0);
		myHalfSizeTarget->SetAsResource(1);
		myPostProcessRenderer.Render(PostProcessRenderer::PP_BLOOM, myIntermediateTargetA->GetWidth(), myIntermediateTargetA->GetHeight());
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
	if (myInputHandler.IsKeyPressed(KeyCode::Z) && myInputHandler.IsKeyDown(KeyCode::CONTROL))
	{
		CommandManager::Undo();
	}
	if (myInputHandler.IsKeyPressed(KeyCode::Y) && myInputHandler.IsKeyDown(KeyCode::CONTROL))
	{
		CommandManager::Redo();
	}

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

void GraphicsEngine::SetSamplerState(SamplerState aState, uint32_t aSlot)
{
	DX11::myContext->PSSetSamplers(aSlot, 1, mySamplerStates[aState].GetAddressOf());
}

void GraphicsEngine::ResetStates()
{
	SetBlendState(BlendState::BS_None);
	SetDepthStencilState(DSS_ReadWrite);
	SetSamplerState(SS_Default, 0);
	SetSamplerState(SS_Wrap, 1);
	SetSamplerState(SS_PointClamp, 2);
	SetSamplerState(SS_PointWrap, 3);

	myIntermediateTargetB->ClearRTV();
	myIntermediateTargetA->ClearRTV();
	myHalfSizeTarget->ClearRTV();
	myQuarterSizeTarget->ClearRTV();
	myBlurTargetA->ClearRTV();
	myBlurTargetB->ClearRTV();
}
