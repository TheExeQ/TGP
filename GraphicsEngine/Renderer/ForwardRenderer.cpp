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

	bufferDesc.ByteWidth = sizeof(SceneLightBufferData);
	result = DX11::myDevice->CreateBuffer(&bufferDesc, nullptr, mySceneLightBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ForwardRenderer::RenderModels(Entity aCamera, std::vector<Entity>& aModelList, std::vector<Entity>& aLightList,
	const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& aEnvironmentLight)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE frameBufferData;

	if (!aCamera.IsValid()) { return; }

	auto& cameraComponent = aCamera.GetComponent<CameraComponent>();
	auto& cameraTransform = aCamera.GetComponent<TransformComponent>();

	myFrameBufferData.View = Matrix4x4<float>::GetFastInverse(cameraTransform.GetTransform());
	myFrameBufferData.Projection = cameraComponent.camera.GetProjectionMatrix();
	myFrameBufferData.CamTranslation = cameraTransform.position;

	result = DX11::myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &frameBufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(frameBufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::myContext->Unmap(myFrameBuffer.Get(), 0);
	DX11::myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	DX11::myContext->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	if (aDirectionalLight)
	{
		mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
	}
	if (aEnvironmentLight)
	{
		aEnvironmentLight->SetAsResource(mySceneLightBuffer);
	}

	mySceneLightBufferData.numLights = 0;
	ZeroMemory(mySceneLightBufferData.Lights, sizeof(Light::LightBufferData) * MAX_FORWARD_LIGHTS);

	for (size_t l = 0; l < aLightList.size() && l < MAX_FORWARD_LIGHTS; l++)
	{
		mySceneLightBufferData.Lights[l] = aLightList[l].GetComponent<LightComponent>().light.GetLightBufferData();
		mySceneLightBufferData.Lights[l].Position = aLightList[l].GetComponent<TransformComponent>().position;
		mySceneLightBufferData.numLights++;
	}

	D3D11_MAPPED_SUBRESOURCE lightBufferData;
	result = DX11::myContext->Map(mySceneLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightBufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(lightBufferData.pData, &mySceneLightBufferData, sizeof(SceneLightBufferData));
	DX11::myContext->Unmap(mySceneLightBuffer.Get(), 0);

	DX11::myContext->PSSetConstantBuffers(3, 1, mySceneLightBuffer.GetAddressOf());
	
	for (auto& model : aModelList)
	{
		auto& modelInst = model.GetComponent<ModelComponent>().modelInstance;
		auto& modelTransform = model.GetComponent<TransformComponent>();
		auto modelRef = modelInst.GetModel();

		D3D11_MAPPED_SUBRESOURCE objBufferData;
		ZeroMemory(&objBufferData, sizeof(objBufferData));

		ZeroMemory(&myObjectBufferData.HasBones, 16);

		myObjectBufferData.World = modelTransform.GetTransform();
		myObjectBufferData.HasBones = modelRef->HasBones();
		myObjectBufferData.IsInstanced = modelInst.HasRenderedInstances();
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

			DX11::myContext->IASetIndexBuffer(modelData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			DX11::myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(modelData.myPrimitiveTopology));
			DX11::myContext->IASetInputLayout(modelData.myInputLayout.Get());

			DX11::myContext->VSSetShader(modelData.myVS.Get(), nullptr, 0);
			DX11::myContext->GSSetShader(nullptr, nullptr, 0);
			DX11::myContext->PSSetShader(modelData.myPS.Get(), nullptr, 0);

			DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::myContext->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			DX11::myContext->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

			if (model.GetComponent<ModelComponent>().modelInstance.HasRenderedInstances())
			{
				const auto& mdlInst = model.GetComponent<ModelComponent>().modelInstance;
				ID3D11Buffer* buffers[2] = { modelData.myVertexBuffer.Get(), mdlInst.GetInstanceBuffer().Get() };
				UINT stride[2] = { modelData.myStride, sizeof(ModelInstance::RenderedInstanceData) };
				UINT offset[2] = { 0, 0 };

				DX11::myContext->IASetVertexBuffers(0, 2, buffers, stride, offset);
				DX11::myContext->DrawIndexedInstanced(modelData.myIndexCount, mdlInst.GetNumOfInstances(), 0, 0, 0);
			}
			else
			{
				DX11::myContext->IASetVertexBuffers(0, 1, modelData.myVertexBuffer.GetAddressOf(), &modelData.myStride, &modelData.myOffset);
				DX11::myContext->DrawIndexed(modelData.myIndexCount, 0, 0);
			}
		}
	}
}

void ForwardRenderer::RenderParticles(Entity aCamera, std::vector<Entity>& aParticleSystemList)
{
	HRESULT result = S_FALSE;
	D3D11_MAPPED_SUBRESOURCE frameBufferData;

	if (!aCamera.IsValid()) { return; }

	auto& cameraComponent = aCamera.GetComponent<CameraComponent>();
	auto& cameraTransform = aCamera.GetComponent<TransformComponent>();

	myFrameBufferData.View = Matrix4x4<float>::GetFastInverse(cameraTransform.GetTransform());
	myFrameBufferData.Projection = cameraComponent.camera.GetProjectionMatrix();
	myFrameBufferData.CamTranslation = cameraTransform.position;

	result = DX11::myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &frameBufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(frameBufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::myContext->Unmap(myFrameBuffer.Get(), 0);

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

		myObjectBufferData.World = transformComponent.GetTransform();
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
