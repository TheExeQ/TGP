#pragma once
#include <vector>

struct FBXVertex
{
	float Position[4] = { 0.f, 0.f, 0.f, 1.f };
	float VertexColors[4][4];
	float UVs[4][2];
	float Normal[3] = { 0.f, 0.f, 0.f };
	float Tangent[3] = { 0.f, 0.f, 0.f };
	float Bitangent[3] = { 0.f, 0.f, 0.f };
	
	uint32_t BoneIDs[4] = { 0, 0, 0, 0 };
	float BoneWeights[4] = { 0.f, 0.f, 0.f, 0.f };
};

struct FBXModel
{
	struct FBXMesh
	{
		std::vector<FBXVertex> vertices;
		std::vector<uint32_t> indices;
	};
};