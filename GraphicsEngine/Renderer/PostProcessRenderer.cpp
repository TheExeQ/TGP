#include "PostProcessRenderer.h"

#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"

#include <fstream>

bool PostProcessRenderer::Init()
{
	HRESULT result = S_FALSE;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDesc.ByteWidth = sizeof(FrameBufferData);
	result = DX11::myDevice->CreateBuffer(&bufferDesc, nullptr, myFrameBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	std::ifstream vsFile;

	vsFile.open("../Assets/Shaders/FullscreenVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	result = DX11::myDevice->CreateVertexShader(vsData.data(), vsData.size(), 0, &myFullscreenVS);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::array<std::string, PP_COUNT> shaderPaths;
	shaderPaths[PP_COPY] = "PP-CopyPS.cso";
	shaderPaths[PP_LUMINANCE] = "PP-LuminancePS.cso";
	shaderPaths[PP_GAUSSIAN] = "PP-GaussianPS.cso";
	shaderPaths[PP_BLOOM] = "PP-BloomPS.cso";
	shaderPaths[PP_SSAO] = "PP-ScreenSpaceAmbientOcclusionPS.cso";

	for (uint32_t i = 0; i < PP_COUNT; i++)
	{
		std::ifstream psFile;

		psFile.open(std::string("../Assets/Shaders/") + shaderPaths[i], std::ios::binary);
		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

		result = DX11::myDevice->CreatePixelShader(psData.data(), psData.size(), 0, myPassShaders[i].GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
		psFile.close();
	}

    return true;
}

void PostProcessRenderer::Render(PostProcessPass aPass, Entity aCameraEntity)
{
	D3D11_MAPPED_SUBRESOURCE frameBufferData;

	myFrameBufferData.Resolution = CommonUtilities::Vector2<float>(0.f, 0.f); // IDK why this works
	if (aCameraEntity.IsValid() && aCameraEntity.HasComponent<CameraComponent>())
	{
		const auto& camera = aCameraEntity.GetComponent<CameraComponent>().camera;
		const auto& cameraTransform = aCameraEntity.GetComponent<TransformComponent>();
		myFrameBufferData.View = CommonUtilities::Matrix4x4<float>::GetFastInverse(cameraTransform.GetTransform());
		myFrameBufferData.CamTranslation = cameraTransform.position;
		myFrameBufferData.nearPlane = camera.GetNear();
		myFrameBufferData.farPlane = camera.GetFar();

		const float aspectRatio = (float)myFrameBufferData.Resolution.x / (float)myFrameBufferData.Resolution.y;
		const float farp = camera.GetFar();
		const float halfHeight = farp * std::tanf(0.5f * camera.GetFov());
		const float halfWidth = aspectRatio * halfHeight;

		myFrameBufferData.FrustumCorners[0] = Vector4<float>(-halfWidth, -halfHeight, farp, 0.f);
		myFrameBufferData.FrustumCorners[1] = Vector4<float>(-halfWidth, halfHeight, farp, 0.f);
		myFrameBufferData.FrustumCorners[2] = Vector4<float>(halfWidth, halfHeight, farp, 0.f);
		myFrameBufferData.FrustumCorners[3] = Vector4<float>(halfWidth, -halfHeight, farp, 0.f);

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		DX11::mySwapchain->GetDesc(&SwapChainDesc);
		const HWND hwnd = SwapChainDesc.OutputWindow;
		CommonUtilities::Vector2<float> res = CommonUtilities::Vector2<float>(DX11::myClientRect.right - DX11::myClientRect.left, DX11::myClientRect.bottom - DX11::myClientRect.top);

		myFrameBufferData.Resolution = res;
	}

	auto result = DX11::myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &frameBufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(frameBufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
	DX11::myContext->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	DX11::myContext->Unmap(myFrameBuffer.Get(), 0);
	DX11::myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	DX11::myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::myContext->IASetInputLayout(nullptr);

	DX11::myContext->VSSetShader(myFullscreenVS.Get(), nullptr, 0);
	DX11::myContext->GSSetShader(nullptr, nullptr, 0);
	DX11::myContext->PSSetShader(myPassShaders[aPass].Get(), nullptr, 0);

	DX11::myContext->Draw(3, 0);
}
