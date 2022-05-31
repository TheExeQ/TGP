#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <fstream>
#include "Vertex.h"
#include "Camera.h"
#include "ModelInstance.h"
#include "CU/TimeHandler.hpp"

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY,
	unsigned someWidth, unsigned someHeight,
	bool enableDeviceDebug)
{
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
	
	if (!InitializeScene())
	{
		return false;
	}

	return true;
}

bool GraphicsEngine::InitializeScene()
{	
	myScene = std::make_shared<Scene>();
	auto camera = std::make_shared<Camera>();
	camera->SetProjectionValues(90, 9.f/16.f, 0.1f, 10000.0f);
	camera->SetPosition(0.0f, 25.0f, -100.0f);
	myScene->SetMainCamera(camera);

	//myModelAssetHandler.Init();
	//auto mdlChest = myModelAssetHandler.GetModelInstance("Cube");

	myModelAssetHandler.LoadModel("Models/SK/gremlin_sk.fbx");
	myModelAssetHandler.LoadAnimation("Models/SK/gremlin_sk.fbx", "Models/Animations/gremlin@run.fbx");
	std::shared_ptr<ModelInstance> gremlin = myModelAssetHandler.GetModelInstance("Models/SK/gremlin_sk.fbx");
	gremlin->SetAnimation("Models/Animations/gremlin@run.fbx");
	gremlin->SetAnimationState(eAnimationState::Playing);
	myScene->AddGameObject(gremlin);
	return true;
}

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	Time::Update();

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
	myFramework.BeginFrame();
}

void GraphicsEngine::RenderFrame()
{
	// Will be fleshed out later!
	if (myScene)
	{
		const std::shared_ptr<Camera> camera = myScene->GetMainCamera();
		const std::vector<std::shared_ptr<Model>> modelsToRender = myScene->CullModels(camera);
		modelsToRender[0]->SetRotation(
			modelsToRender[0]->GetTransform().myRotation.x,
			180.f,
			modelsToRender[0]->GetTransform().myRotation.z
		);
		myForwardRenderer.Render(camera, modelsToRender);
	}
}

void GraphicsEngine::EndFrame()
{
	// F1 - This is where we finish our rendering and tell the framework
	// to present our result to the screen.
	myFramework.EndFrame();
}
