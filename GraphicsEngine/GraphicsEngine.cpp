#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <fstream>
#include "Vertex.h"
#include "Camera.h"
#include "ModelInstance.h"
#include "CU/Timer.hpp"

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

	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = GraphicsEngine::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"TGP";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"TGP",
		L"Till�mpad Grafikprogrammering",
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
	myFramework.BeginFrame();
}

void GraphicsEngine::RenderFrame()
{
	myTimer.Update();

	// Will be fleshed out later!
	if (myScene)
	{
		Controller();

		const std::shared_ptr<Camera> camera = myScene->GetMainCamera();
		const std::vector<std::shared_ptr<Model>> modelsToRender = myScene->CullModels(camera);
		myForwardRenderer.Render(camera, modelsToRender);
	}
}

void GraphicsEngine::EndFrame()
{
	// F1 - This is where we finish our rendering and tell the framework
	// to present our result to the screen.
	myFramework.EndFrame();
}

void GraphicsEngine::Controller()
{
	static float moveSpeed = 100.f;
	static float mouseSens = 10.f;

	if (myInputHandler.IsKeyDown(KeyCode::W))
	{
		myScene->GetMainCamera()->MoveForward(moveSpeed * myTimer.GetDeltaTime());
	}
	if (myInputHandler.IsKeyDown(KeyCode::A))
	{
		myScene->GetMainCamera()->MoveRight(-moveSpeed * myTimer.GetDeltaTime());
	}
	if (myInputHandler.IsKeyDown(KeyCode::S))
	{
		myScene->GetMainCamera()->MoveForward(-moveSpeed * myTimer.GetDeltaTime());
	}
	if (myInputHandler.IsKeyDown(KeyCode::D))
	{
		myScene->GetMainCamera()->MoveRight(moveSpeed * myTimer.GetDeltaTime());
	}

	if (myInputHandler.IsKeyDown(KeyCode::E))
	{
		myScene->GetMainCamera()->AdjustPosition(0.f, moveSpeed * myTimer.GetDeltaTime(), 0.f);
	}
	if (myInputHandler.IsKeyDown(KeyCode::Q))
	{
		myScene->GetMainCamera()->AdjustPosition(0.f, -moveSpeed * myTimer.GetDeltaTime(), 0.f);
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
			myScene->GetMainCamera()->AdjustRotation(deltaY * mouseSens * myTimer.GetDeltaTime(),
				deltaX * mouseSens * myTimer.GetDeltaTime(), 0.f);
		}
	}
	prevFrame = myInputHandler.GetMousePosition();
}