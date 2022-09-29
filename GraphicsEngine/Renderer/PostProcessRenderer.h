#pragma once
#include "ConstantBufferTypes.h"
#include "Core/DX11.h"

#include <array>

class PostProcessRenderer
{
public:
	enum PostProcessPass
	{
		PP_COPY,
		PP_LUMINANCE,
		PP_GAUSSIAN,
		PP_BLOOM,
		PP_COUNT
	};

public:
	bool Init();
	void Render(PostProcessPass aPass);

private:
	FrameBufferData myFrameBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11VertexShader> myFullscreenVS;

	std::array<ComPtr<ID3D11PixelShader>, PostProcessPass::PP_COUNT> myPassShaders;
};