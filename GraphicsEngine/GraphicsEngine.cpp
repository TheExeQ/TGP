#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <fstream>
#include "Vertex.h"
#include "Camera.h"

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
	myFramework.Initialize(myWindowHandle, false);

	return true;
}

bool GraphicsEngine::InitializeScene()
{
	Model::ModelData modelData;
	
	auto model = std::make_shared<Model>();

	std::vector<Vertex> mdlVertices =
	{
		Vertex(50.f, 50.f, 50.f, 1.f, 0.f, 0.f, 1.f),
		Vertex(50.f, 50.f, -50.f, 0.f, 1.f, 0.f, 1.f),
		Vertex(50.f, -50.f, 50.f, 0.f, 0.f, 1.f, 1.f),
		Vertex(-50.f, 50.f, 50.f, 1.f, 0.f, 1.f, 1.f),

		Vertex(50.f, -50.f, -50.f, 0.f, 1.f, 1.f, 1.f),
		Vertex(-50.f, 50.f, -50.f, 1.f, 1.f, 0.f, 1.f),
		Vertex(-50.f, -50.f, 50.f, 1.f, 1.f, 1.f, 1.f),
		Vertex(-50.f, -50.f, -50.f, 0.f, 0.f, 0.f, 1.f),
	};

	std::vector<int> mdlIndices =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 1,
		1, 7, 6,
		2, 1, 7,
		2, 7, 4,
		2, 4, 3,
		3, 4, 7,
		3, 7, 6,
		3, 6, 5,
		5, 6, 7,
		5, 7, 1,
		5, 1, 2,
		5, 2, 3,
		5, 3, 4,
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * mdlVertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexSubData;
	vertexSubData.pSysMem = &mdlVertices[0];

	HRESULT result = DX11::myDevice->CreateBuffer(&vertexBufferDesc, &vertexSubData, modelData.myVertexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * mdlIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexSubData;
	indexSubData.pSysMem = &mdlIndices[0];

	result = DX11::myDevice->CreateBuffer(&indexBufferDesc, &indexSubData, modelData.myIndexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	std::ifstream vsFile;
	
	vsFile.open("DefaultVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;

	result = DX11::myDevice->CreateVertexShader(vsData.data(), vsData.size(), 0, &vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;

	psFile.open("DefaultPS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;

	result = DX11::myDevice->CreatePixelShader(psData.data(), psData.size(), 0, &pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	psFile.close();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	result = DX11::myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), modelData.myInputLayout.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Continue at page 39

	model->Init(modelData, "");
	
	auto camera = std::make_shared<Camera>();

	//myScene.AddGameObject(model);
	myScene.SetCamera(camera);
	
	return true;
}

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
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
}

void GraphicsEngine::EndFrame()
{
	// F1 - This is where we finish our rendering and tell the framework
	// to present our result to the screen.
	myFramework.EndFrame();
}
