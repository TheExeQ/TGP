#pragma once
#include "Math/Vector.hpp"

using namespace CommonUtilities;

struct Vertex
{
	Vertex()
	{
		Position.x = 0.f;
		Position.y = 0.f;
		Position.z = 0.f;
		Position.w = 1.f;
		VertexColors[0] = { 0.f, 0.f, 0.f, 1.f };
	};
	Vertex(float x, float y, float z, float r, float g, float b, float a,
		float u, float v,
		Vector3<float> aTangent, Vector3<float> aBinormal, Vector3<float> aNormal)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;
		VertexColors[0] = { r, g, b, a };

		UVs[0] = { u, v };

		Tangent = aTangent;
		Binormal = aBinormal;
		Normal = aNormal;
	};

	Vector4<float> Position = { 0.f, 0.f, 0.f, 1.f };
	Vector4<float> VertexColors[4] =
	{
		{ 0.f, 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f, 0.f }
	};

	Vector2<float> UVs[4] = 
	{
		{0.f, 0.f},
		{0.f, 0.f},
		{0.f, 0.f},
		{0.f, 0.f},
	};

	Vector4<unsigned int> BoneIDs = { 0, 0, 0, 0 };
	Vector4<float> BoneWeights = { 0.f, 0.f, 0.f, 0.f };

	Vector3<float> Tangent = { 0.f, 0.f, 0.f };
	Vector3<float> Binormal = { 0.f, 0.f, 0.f };
	Vector3<float> Normal = { 0.f, 0.f, 0.f };
};