#include "ModelAssetHandler.h"
#include "Vertex.h"
#include "DX11.h"
#include <fstream>

std::unordered_map<std::string, std::shared_ptr<Model>> ModelAssetHandler::myModelRegistry;

bool ModelAssetHandler::Init()
{
	InitUnitCube();
	return true;
}

std::shared_ptr<Model> ModelAssetHandler::GetModel(const std::string& name) const
{
	return myModelRegistry[name];
}

bool ModelAssetHandler::InitUnitCube()
{
	Model::ModelData modelData;

	//std::vector<Vertex> mdlVertices =
	//{
	//	Vertex(50.f, 50.f, 50.f, 1.f, 0.f, 0.f, 1.f),
	//	Vertex(50.f, 50.f, -50.f, 0.f, 1.f, 0.f, 1.f),
	//	Vertex(50.f, -50.f, 50.f, 0.f, 0.f, 1.f, 1.f),
	//	Vertex(-50.f, 50.f, 50.f, 1.f, 0.f, 1.f, 1.f),

	//	Vertex(50.f, -50.f, -50.f, 0.f, 1.f, 1.f, 1.f),
	//	Vertex(-50.f, 50.f, -50.f, 1.f, 1.f, 0.f, 1.f),
	//	Vertex(-50.f, -50.f, 50.f, 1.f, 1.f, 1.f, 1.f),
	//	Vertex(-50.f, -50.f, -50.f, 0.f, 0.f, 0.f, 1.f),
	//};

	//std::vector<int> mdlIndices =
	//{
	//	0, 1, 2,
	//	0, 2, 3,
	//	0, 4, 1,
	//	4, 5, 1,
	//	3, 2, 6,
	//	3, 6, 7,
	//	0, 3, 4,
	//	4, 3, 7,
	//	1, 6, 2,
	//	1, 5, 6,
	//	4, 7, 5,
	//	7, 5, 6,
	//};

	std::vector<Vertex> mdlVertices =
	{
		//Front
		Vertex(-50.f, -50.f, -50.f, 0.f, 0.f, 0.f, 1.f), // Down Left 
		Vertex(-50.f, 50.f, -50.f, 1.f, 1.f, 0.f, 1.f), // Up Left
		Vertex(50.f, 50.f, -50.f, 0.f, 1.f, 0.f, 1.f), // Up Right
		Vertex(50.f, -50.f, -50.f, 0.f, 1.f, 1.f, 1.f), // Down Right
		
		// Back
		Vertex(-50.f, -50.f, 50.f, 1.f, 1.f, 1.f, 1.f), // Down Left
		Vertex(-50.f, 50.f, 50.f, 1.f, 0.f, 1.f, 1.f), // Up Left
		Vertex(50.f, 50.f, 50.f, 1.f, 0.f, 0.f, 1.f), // Up Right
		Vertex(50.f, -50.f, 50.f, 0.f, 0.f, 1.f, 1.f), // Down Right
	};

	std::vector<int> mdlIndices =
	{
		//Front
		0, 1, 2,
		0, 2, 3,

		// Back
		4, 5, 6,
		4, 6, 7,

		// Left
		0, 4, 5,
		0, 5, 1,

		// Right
		2, 6, 7,
		2, 7, 3,

		// Top
		1, 5, 6,
		1, 6, 2,

		// Bottom
		4, 0, 3,
		4, 3, 7,
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

	vsFile.open("../Assets/Shaders/DefaultVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	result = DX11::myDevice->CreateVertexShader(vsData.data(), vsData.size(), 0, &modelData.myVS);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream psFile;

	psFile.open("../Assets/Shaders/DefaultPS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	result = DX11::myDevice->CreatePixelShader(psData.data(), psData.size(), 0, &modelData.myPS);
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

	modelData.myVertexCount = mdlVertices.size();
	modelData.myIndexCount = mdlIndices.size();
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myStride = sizeof(Vertex);
	modelData.myOffset = 0;

	auto mdl = std::make_shared<Model>();
	
	mdl->Init(modelData, "Cube");
	myModelRegistry.insert({ "Cube", mdl });
	return true;
}
