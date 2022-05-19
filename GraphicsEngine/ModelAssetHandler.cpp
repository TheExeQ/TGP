#include "ModelAssetHandler.h"
#include <fstream>
#include "Vertex.h"
#include "DX11.h"
#include "Model.h"
#include "ModelInstance.h"
#include "FBXImporter/FBXImporter.h"
#include "Random.h"

std::unordered_map<std::string, std::shared_ptr<Model>> ModelAssetHandler::myModelRegistry;

bool ModelAssetHandler::Init()
{
	InitUnitCube();
	return true;
}

std::shared_ptr<ModelInstance> ModelAssetHandler::GetModelInstance(const std::string& name) const
{
	std::shared_ptr<ModelInstance> modelInstance = std::make_shared<ModelInstance>();
	modelInstance->Init(myModelRegistry[name]);

	return modelInstance;
}

bool ModelAssetHandler::LoadModel(const std::string& someFilePath)
{
	std::vector<Model::ModelData> modelDataVector;
	const std::string filePath = "../Assets/" + someFilePath;
	TGA::FBXModel tgaModel;
	if (TGA::FBXImporter::LoadModel(filePath, tgaModel))
	{
		Skeleton mdlSkeleton;
		const bool hasSkeleton = tgaModel.Skeleton.GetRoot();
		if (hasSkeleton)
		{
			for (size_t j = 0; j < tgaModel.Skeleton.Bones.size(); ++j)
			{
				
			}
		}

		Model::ModelData modelData;
		std::vector<Model::ModelData> mdlData;
		mdlData.resize(tgaModel.Meshes.size());
		for (size_t i = 0; i < tgaModel.Meshes.size(); i++)
		{
			TGA::FBXModel::FBXMesh& mesh = tgaModel.Meshes[i];

			std::vector<Vertex> mdlVertices;
			mdlVertices.resize(mesh.Vertices.size());
			std::vector<uint32_t> mdlIndices = mesh.Indices;
			
			for (size_t v = 0; v < mesh.Vertices.size(); v++)
			{
				mdlVertices[v].Position.x = mesh.Vertices[v].Position[0];
				mdlVertices[v].Position.y = mesh.Vertices[v].Position[1];
				mdlVertices[v].Position.z = mesh.Vertices[v].Position[2];
				mdlVertices[v].Position.w = mesh.Vertices[v].Position[3];

				mdlVertices[v].VertexColors[0] = { Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), 1.f };
				mdlVertices[v].VertexColors[1] = { Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), 1.f };
				mdlVertices[v].VertexColors[2] = { Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), 1.f };
				mdlVertices[v].VertexColors[3] = { Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), Random::GetRandomFloat(0.f, 1.f), 1.f };

				if (hasSkeleton)
				{
					mdlVertices[v].BoneIDs.x = mesh.Vertices[v].BoneIDs[0];
					mdlVertices[v].BoneIDs.y = mesh.Vertices[v].BoneIDs[1];
					mdlVertices[v].BoneIDs.z = mesh.Vertices[v].BoneIDs[2];
					mdlVertices[v].BoneIDs.w = mesh.Vertices[v].BoneIDs[3];

					mdlVertices[v].BoneWeights.x = mesh.Vertices[v].BoneWeights[0];
					mdlVertices[v].BoneWeights.y = mesh.Vertices[v].BoneWeights[1];
					mdlVertices[v].BoneWeights.z = mesh.Vertices[v].BoneWeights[2];
					mdlVertices[v].BoneWeights.w = mesh.Vertices[v].BoneWeights[3];
				}
			}
			
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
			indexBufferDesc.ByteWidth = sizeof(uint32_t) * mdlIndices.size();
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
				{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
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

			modelDataVector.push_back(modelData);
		}
		auto mdl = std::make_shared<Model>();

		if (hasSkeleton)
		{
			mdl->Init(modelDataVector, someFilePath, mdlSkeleton);
		}
		else
		{
			mdl->Init(modelDataVector, someFilePath);
		}

		myModelRegistry.insert({ someFilePath, mdl });
		return true;
	}
	return false;
}

bool ModelAssetHandler::LoadAnimation(const std::string& aModelName, const std::string& someFilePath) const
{
	auto model = myModelRegistry[aModelName];
	TGA::FBXAnimation tgaAnimation;
	if (TGA::FBXImporter::LoadAnimation("../Assets/" + someFilePath, model->GetSkeleton()->BoneNames, tgaAnimation))
	{
		Animation result;
		result.Frames.resize(tgaAnimation.Frames.size());

		for (size_t f = 0; f < result.Frames.size(); f++)
		{
			for (size_t i = 0; i < tgaAnimation.Frames[f].LocalTransforms.size(); i++)
			{
				result.Frames[f].LocalTransforms[i](1, 1) = tgaAnimation.Frames[f].LocalTransforms[i].Data[0];
				result.Frames[f].LocalTransforms[i](1, 2) = tgaAnimation.Frames[f].LocalTransforms[i].Data[1];
				result.Frames[f].LocalTransforms[i](1, 3) = tgaAnimation.Frames[f].LocalTransforms[i].Data[2];
				result.Frames[f].LocalTransforms[i](1, 4) = tgaAnimation.Frames[f].LocalTransforms[i].Data[3];

				result.Frames[f].LocalTransforms[i](2, 1) = tgaAnimation.Frames[f].LocalTransforms[i].Data[4];
				result.Frames[f].LocalTransforms[i](2, 2) = tgaAnimation.Frames[f].LocalTransforms[i].Data[5];
				result.Frames[f].LocalTransforms[i](2, 3) = tgaAnimation.Frames[f].LocalTransforms[i].Data[6];
				result.Frames[f].LocalTransforms[i](2, 4) = tgaAnimation.Frames[f].LocalTransforms[i].Data[7];

				result.Frames[f].LocalTransforms[i](3, 1) = tgaAnimation.Frames[f].LocalTransforms[i].Data[8];
				result.Frames[f].LocalTransforms[i](3, 2) = tgaAnimation.Frames[f].LocalTransforms[i].Data[9];
				result.Frames[f].LocalTransforms[i](3, 3) = tgaAnimation.Frames[f].LocalTransforms[i].Data[10];
				result.Frames[f].LocalTransforms[i](3, 4) = tgaAnimation.Frames[f].LocalTransforms[i].Data[11];
													
				result.Frames[f].LocalTransforms[i](4, 1) = tgaAnimation.Frames[f].LocalTransforms[i].Data[12];
				result.Frames[f].LocalTransforms[i](4, 2) = tgaAnimation.Frames[f].LocalTransforms[i].Data[13];
				result.Frames[f].LocalTransforms[i](4, 3) = tgaAnimation.Frames[f].LocalTransforms[i].Data[14];
				result.Frames[f].LocalTransforms[i](4, 4) = tgaAnimation.Frames[f].LocalTransforms[i].Data[15];
			}
		}
		
		model->AddAnimation(result);
		return true;
	}
	return false;
}

bool ModelAssetHandler::InitUnitCube()
{
	std::vector<Model::ModelData> modelDataVector;
	Model::ModelData modelData;

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
		6, 5, 4,
		7, 6, 4,

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
	
	modelDataVector.push_back(modelData);

	mdl->Init(modelDataVector, "Cube");
	myModelRegistry.insert({ "Cube", mdl });
	return true;
}
