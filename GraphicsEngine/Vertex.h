#pragma once
#include "Math/Vector4.hpp"

using namespace CommonUtilities;

struct Vertex
{
	Vertex(float x, float y, float z, float r, float g, float b, float a)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;
		VertexColors[0] = { r, g, b, a };
	};

	Vector4<float> Position = { 0.f, 0.f, 0.f, 1.f };
	Vector4<float>  VertexColors[4] =
	{
		{ 0.f, 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f, 0.f }
	};
};