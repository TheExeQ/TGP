#include "ModelAssetHandler.h"
#include <filesystem>
#include <fstream>
#include "Vertex.h"
#include "Core/DX11.h"
#include "Model.h"
#include "ModelInstance.h"
#include "FBXImporter/FBXImporter.h"
#include "Core/Random.h"
#include "Material.h"
#include "TextureAssetHandler.h"

std::unordered_map<std::string, std::shared_ptr<Model>> ModelAssetHandler::myModelRegistry;
std::unordered_map<std::string, std::shared_ptr<Material>> ModelAssetHandler::myMaterialRegistry;

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
			mdlSkeleton.Name = tgaModel.Skeleton.Name;
			mdlSkeleton.Bones.resize(tgaModel.Skeleton.Bones.size());
			mdlSkeleton.BoneNames.resize(tgaModel.Skeleton.Bones.size());
			for (size_t j = 0; j < tgaModel.Skeleton.Bones.size(); ++j)
			{
				mdlSkeleton.Bones[j].Name = tgaModel.Skeleton.Bones[j].Name;
				mdlSkeleton.BoneNames[j] = tgaModel.Skeleton.Bones[j].Name;
				mdlSkeleton.BoneNameToIndex[tgaModel.Skeleton.Bones[j].Name] = j;

				mdlSkeleton.Bones[j].Parent = tgaModel.Skeleton.Bones[j].Parent;
				mdlSkeleton.Bones[j].Children = tgaModel.Skeleton.Bones[j].Children;

				mdlSkeleton.Bones[j].BindPoseInverse(1, 1) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[0];
				mdlSkeleton.Bones[j].BindPoseInverse(1, 2) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[1];
				mdlSkeleton.Bones[j].BindPoseInverse(1, 3) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[2];
				mdlSkeleton.Bones[j].BindPoseInverse(1, 4) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[3];
				
				mdlSkeleton.Bones[j].BindPoseInverse(2, 1) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[4];
				mdlSkeleton.Bones[j].BindPoseInverse(2, 2) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[5];
				mdlSkeleton.Bones[j].BindPoseInverse(2, 3) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[6];
				mdlSkeleton.Bones[j].BindPoseInverse(2, 4) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[7];
				
				mdlSkeleton.Bones[j].BindPoseInverse(3, 1) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[8];
				mdlSkeleton.Bones[j].BindPoseInverse(3, 2) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[9];
				mdlSkeleton.Bones[j].BindPoseInverse(3, 3) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[10];
				mdlSkeleton.Bones[j].BindPoseInverse(3, 4) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[11];
				
				mdlSkeleton.Bones[j].BindPoseInverse(4, 1) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[12];
				mdlSkeleton.Bones[j].BindPoseInverse(4, 2) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[13];
				mdlSkeleton.Bones[j].BindPoseInverse(4, 3) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[14];
				mdlSkeleton.Bones[j].BindPoseInverse(4, 4) = tgaModel.Skeleton.Bones[j].BindPoseInverse.Data[15];
			}
		}

		Model::ModelData modelData;
		std::vector<Model::ModelData> mdlData;
		mdlData.resize(tgaModel.Meshes.size());
		for (size_t i = 0; i < tgaModel.Meshes.size(); i++)
		{
			TGA::FBXModel::FBXMesh& mesh = tgaModel.Meshes[i];

			const std::string matName = mesh.MaterialName;
			std::shared_ptr<Material> meshMaterial;
			if (myMaterialRegistry.find(matName) != myMaterialRegistry.end())
			{
				meshMaterial = myMaterialRegistry[matName];
			}
			else
			{
				meshMaterial = std::make_shared<Material>();
				CommonUtilities::Vector3<float> albedo;
				albedo.x = Random::GetRandomFloat(0.f, 1.f);
				albedo.y = Random::GetRandomFloat(0.f, 1.f);
				albedo.z = Random::GetRandomFloat(0.f, 1.f);
				meshMaterial->Init(matName, albedo);
			}

			std::filesystem::path p(someFilePath);
			const std::string baseFileName = p.stem().string();

			const std::string albedoFileName = "T_" + baseFileName + "_C.dds";
			const std::string normalFileName = "T_" + baseFileName + "_N.dds";
			const std::string materialFileName = "T_" + baseFileName + "_M.dds";

			if (TextureAssetHandler::LoadTexture(albedoFileName))
			{
				meshMaterial->SetAlbedoTexture(TextureAssetHandler::GetTexture(albedoFileName));
			}

			if (TextureAssetHandler::LoadTexture(normalFileName))
			{
				meshMaterial->SetNormalTexture(TextureAssetHandler::GetTexture(normalFileName));
			}

			if (TextureAssetHandler::LoadTexture(materialFileName))
			{
				meshMaterial->SetMaterialTexture(TextureAssetHandler::GetTexture(materialFileName));
			}

			myMaterialRegistry.insert({ matName, meshMaterial });
			modelData.myMaterial = meshMaterial;

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

				for (int uvCh = 0; uvCh < 4; uvCh++)
				{
					mdlVertices[v].UVs[uvCh] = {mesh.Vertices[v].UVs[uvCh][0], mesh.Vertices[v].UVs[uvCh][1]};
				}

				mdlVertices[v].Tangent =
				{ 
					mesh.Vertices[v].Tangent[0],
					mesh.Vertices[v].Tangent[1],
					mesh.Vertices[v].Tangent[2] 
				};
				mdlVertices[v].Binormal =
				{
					mesh.Vertices[v].Binormal[0],
					mesh.Vertices[v].Binormal[1],
					mesh.Vertices[v].Binormal[2]
				};
				mdlVertices[v].Normal =
				{
					mesh.Vertices[v].Normal[0],
					mesh.Vertices[v].Normal[1],
					mesh.Vertices[v].Normal[2]
				};
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
				{"UVS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UVS", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UVS", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UVS", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
			result.Frames[f].LocalTransforms.resize(tgaAnimation.Frames[f].LocalTransforms.size());
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
				result.Frames[f].LocalTransforms[i] = CommonUtilities::Matrix4x4<float>::Transpose(result.Frames[f].LocalTransforms[i]);
			}
		}
		
		result.Length = tgaAnimation.Length;
		result.Name = tgaAnimation.Name;
		result.FramesPerSecond = tgaAnimation.FramesPerSecond;
		result.Duration = tgaAnimation.Duration;
		result.CurrentFrame = 1;
		result.State = eAnimationState::Stopped;
			
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
		Vertex(-50, 50, -50, 1, 1, 1, 1, 0, 0, {1,0,0}, {0,-1,0}, {0,0,-1}),
		Vertex(50, 50, -50, 1, 1, 1, 1, 1, 0, {1,0,0}, {0,-1,0}, {0,0,-1}),
		Vertex(50, -50, -50, 1, 1, 1, 1, 1, 1, {1,0,0}, {0,-1,0}, {0,0,-1}),
		Vertex(-50, -50, -50, 1, 1, 1, 1, 0, 1, {1,0,0}, {0,-1,0}, {0,0,-1}),
		
		//Back
		Vertex(50, 50, 50, 1, 1, 1, 1, 0, 0, {-1,0,0}, {0,-1,0}, {0,0,1}),
		Vertex(-50, 50, 50, 1, 1, 1, 1, 1, 0, {-1,0,0}, {0,-1,0}, {0,0,1}),
		Vertex(-50, -50, 50, 1, 1, 1, 1, 1, 1, {-1,0,0}, {0,-1,0}, {0,0,1}),
		Vertex(50, -50, 50, 1, 1, 1, 1, 0, 1, {-1,0,0}, {0,-1,0}, {0,0,1}),
		
		//Left
		Vertex(-50, 50, 50, 1, 1, 1, 1, 0, 0, {0,0,1}, {0,-1,0}, {-1,0,0}),
		Vertex(-50, 50, -50, 1, 1, 1, 1, 1, 0, {0,0,1}, {0,-1,0}, {-1,0,0}),
		Vertex(-50, -50, -50, 1, 1, 1, 1, 1, 1, {0,0,1}, {0,-1,0}, {-1,0,0}),
		Vertex(-50, -50, 50, 1, 1, 1, 1, 0, 1, {0,0,1}, {0,-1,0}, {-1,0,0}),
		
		//Right
		Vertex(50, 50, -50, 1, 1, 1, 1, 0, 0, {0,0,-1}, {0,-1,0}, {1,0,0}),
		Vertex(50, 50, 50, 1, 1, 1, 1, 1, 0, {0,0,-1}, {0,-1,0}, {1,0,0}),
		Vertex(50, -50, 50, 1, 1, 1, 1, 1, 1, {0,0,-1}, {0,-1,0}, {1,0,0}),
		Vertex(50, -50, -50, 1, 1, 1, 1, 0, 1, {0,0,-1}, {0,-1,0}, {1,0,0}),
		
		//Top
		Vertex(-50, 50, 50, 1, 1, 1, 1, 0, 0, {1,0,0}, {0,0,1}, {0,1,0}),
		Vertex(50, 50, 50, 1, 1, 1, 1, 1, 0, {1,0,0}, {0,0,1}, {0,1,0}),
		Vertex(50, 50, -50, 1, 1, 1, 1, 1, 1, {1,0,0}, {0,0,1}, {0,1,0}),
		Vertex(-50, 50, -50, 1, 1, 1, 1, 0, 1, {1,0,0}, {0,0,1}, {0,1,0}),
		
		//Bottom
		Vertex(-50, -50, -50, 1, 1, 1, 1, 0, 0, {1,0,0}, {0,0,-1}, {0,-1,0}),
		Vertex(50, -50, -50, 1, 1, 1, 1, 1, 0, {1,0,0}, {0,0,-1}, {0,-1,0}),
		Vertex(50, -50, 50, 1, 1, 1, 1, 1, 1, {1,0,0}, {0,0,-1}, {0,-1,0}),
		Vertex(-50, -50, 50, 1, 1, 1, 1, 0, 1, {1,0,0}, {0,0,-1}, {0,-1,0}),
	};

	std::vector<int> mdlIndices =
	{
		//Front
		0,1,2,
		2,3,0,
		
		//Back
		4,5,6,
		6,7,4,
		
		//Left
		8,9,10,
		10,11,8,
		
		//Right
		12,13,14,
		14,15,12,
		
		//Top
		16,17,18,
		18,19,16,
		
		//Bottom
		20,21,22,
		22,23,20,
	};

	std::shared_ptr<Material> meshMaterial;
	
	meshMaterial = std::make_shared<Material>();

	if (TextureAssetHandler::LoadTexture("T_Default_C.dds"))
	{
		meshMaterial->SetAlbedoTexture(TextureAssetHandler::GetTexture("T_Default_C.dds"));
	}
	
	if (TextureAssetHandler::LoadTexture("T_Default_N.dds"))
	{
		meshMaterial->SetNormalTexture(TextureAssetHandler::GetTexture("T_Default_N.dds"));
	}

	if (TextureAssetHandler::LoadTexture("T_Default_M.dds"))
	{
		meshMaterial->SetMaterialTexture(TextureAssetHandler::GetTexture("T_Default_M.dds"));
	}
	
	myMaterialRegistry.insert({ "Cube", meshMaterial });
	modelData.myMaterial = meshMaterial;

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
		{"UVS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UVS", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UVS", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UVS", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
