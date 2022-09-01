#include "Core/GraphicsEngine.pch.h"
#include "Core/DX11.h"
#include "ForwardRenderer.h"
#include "Camera.h"
#include "ModelInstance.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "EnvironmentLight.h"
#include "ParticleSystem.h"

bool ForwardRenderer::Initialize()
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

	bufferDesc.ByteWidth = sizeof(ObjectBufferData);
	result = DX11::myDevice->CreateBuffer(&bufferDesc, nullptr, myObjectBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	bufferDesc.ByteWidth = sizeof(MaterialBufferData);
	result = DX11::myDevice->CreateBuffer(&bufferDesc, nullptr, myMaterialBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	bufferDesc.ByteWidth = sizeof(Light::LightBufferData);
	result = DX11::myDevice->CreateBuffer(&bufferDesc, nullptr, myLightBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ForwardRenderer::RenderModels(Entity aCamera, std::vector<Entity>& aModelList,
	const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& aEnvironmentLight)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE frameBufferData;

	if (!aCamera.IsValid()) { return; }

	auto& cameraComponent = aCamera.GetComponent<CameraComponent>();
	auto& cameraTransform = aCamera.GetComponent<TransformComponent>();

	myFrameBufferData.View = Matrix4x4<float>::GetFastInverse(cameraTransform.myMatrix);
	myFrameBufferData.Projection = cameraComponent.camera.GetProjectionMatrix();
	myFrameBufferData.CamTranslation = cameraTransform.myPosition;

	result = DX11::myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &frameBufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(frameBufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::myContext->Unmap(myFrameBuffer.Get(), 0);
	DX11::myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	if (aDirectionalLight)
	{
		aDirectionalLight->SetAsResource(myLightBuffer);
	}
	if (aEnvironmentLight)
	{
		aEnvironmentLight->SetAsResource(myLightBuffer);
	}
	
	for (auto& model : aModelList)
	{
		auto& modelInst = model.GetComponent<ModelComponent>().mdlInstance;
		auto& modelTransform = model.GetComponent<TransformComponent>();
		auto modelRef = modelInst.GetModel();

		D3D11_MAPPED_SUBRESOURCE objBufferData;
		ZeroMemory(&objBufferData, sizeof(objBufferData));

		ZeroMemory(&myObjectBufferData.HasBones, 16);

		myObjectBufferData.World = modelTransform.myMatrix;
		myObjectBufferData.HasBones = modelRef->HasBones();
		if (myObjectBufferData.HasBones)
		{
			memcpy_s(&myObjectBufferData.BoneData[0], sizeof(Matrix4x4<float>) * 128,
				&modelRef->GetBoneTransforms()[0], sizeof(Matrix4x4<float>) * 128);
		}
		
		result = DX11::myContext->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &objBufferData);
		if (FAILED(result))
		{
			return;
		}
		memcpy(objBufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));

		DX11::myContext->Unmap(myObjectBuffer.Get(), 0);

		for (uint16_t i = 0; i < modelRef->GetNumMeshes(); ++i)
		{
			const Model::ModelData& modelData = modelRef->GetModelData(i);

			if (modelData.myMaterial)
			{
				modelData.myMaterial->SetAsResource(myMaterialBuffer);
			}
			else
			{
				// Set Default Material
			}

			D3D11_MAPPED_SUBRESOURCE matBufferData;
			ZeroMemory(&matBufferData, sizeof(matBufferData));

			myMaterialBufferData.Albedo = modelData.myMaterial->GetAlbedo();

			result = DX11::myContext->Map(myMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &matBufferData);
			if (FAILED(result))
			{
				return;
			}
			memcpy(matBufferData.pData, &myMaterialBufferData, sizeof(MaterialBufferData));

			DX11::myContext->Unmap(myMaterialBuffer.Get(), 0);

			DX11::myContext->IASetVertexBuffers(0, 1, modelData.myVertexBuffer.GetAddressOf(), &modelData.myStride, &modelData.myOffset);
			DX11::myContext->IASetIndexBuffer(modelData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			DX11::myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(modelData.myPrimitiveTopology));
			DX11::myContext->IASetInputLayout(modelData.myInputLayout.Get());

			DX11::myContext->VSSetShader(modelData.myVS.Get(), nullptr, 0);
			DX11::myContext->GSSetShader(nullptr, nullptr, 0);
			DX11::myContext->PSSetShader(modelData.myPS.Get(), nullptr, 0);

			DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::myContext->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::myContext->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

			DX11::myContext->DrawIndexed(modelData.myIndexCount, 0, 0);
		}
	}
}

void ForwardRenderer::RenderParticles(Entity aCamera, std::vector<Entity>& aParticleSystemList)
{
	HRESULT result = S_FALSE;

	if (!aCamera.IsValid()) { return; }

	DX11::myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::myContext->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::myContext->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	for (auto& entity : aParticleSystemList)
	{
		auto& emitterComponent = entity.GetComponent<ParticleSystemComponent>();
		auto& transformComponent = entity.GetComponent<TransformComponent>();

		D3D11_MAPPED_SUBRESOURCE objBufferData;
		ZeroMemory(&objBufferData, sizeof(objBufferData));
		ZeroMemory(&myObjectBufferData.HasBones, 16);

		myObjectBufferData.World = transformComponent.myMatrix;
		myObjectBufferData.HasBones = false;

		result = DX11::myContext->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &objBufferData);
		if (FAILED(result))
		{
			return;
		}
		memcpy(objBufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));

		DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DX11::myContext->GSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DX11::myContext->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		for (auto& emitter : emitterComponent.system.GetEmitters())
		{
			emitter.SetAsResource();
			DX11::myContext->IASetVertexBuffers(0, 1, emitter.myVertexBuffer.GetAddressOf(), &emitter.myStride, &emitter.myOffset);
			DX11::myContext->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(emitter.myPrimitiveTopology));
			DX11::myContext->IASetInputLayout(emitter.myInputLayout.Get());
			emitter.Draw();
		}
	}
}
