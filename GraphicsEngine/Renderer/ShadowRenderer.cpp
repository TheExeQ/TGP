#include "gepch.h"
#include "ShadowRenderer.h"
#include "Core/DX11.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Material.h"

bool ShadowRenderer::Init()
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

void ShadowRenderer::Render(std::vector<Entity>& aLightList, Ref<DirectionalLight> aDirectionalLight, std::vector<Entity>& aModelList)
{
	myFrameBufferData.View = aDirectionalLight->GetLightBufferData().LightView;
	myFrameBufferData.Projection = aDirectionalLight->GetLightBufferData().LightProj;
	myFrameBufferData.CamTranslation = aDirectionalLight->GetLightBufferData().Position;
	myFrameBufferData.RenderMode = static_cast<unsigned int>(0);

	for (int i = 0; i < aLightList.size() + 1; i++)
	{
		int timesToDraw = 1;

		if (i != 0)
		{
			auto& lightComp = aLightList[i - 1].GetComponent<LightComponent>();
			if (lightComp.light.GetLightBufferData().LightType == 1)
			{
				lightComp.light.ClearShadowMap();
				lightComp.light.SetShadowMapAsDepth(0);
				timesToDraw = 6;
				continue;
			}
			else
			{
				lightComp.light.ClearShadowMap();
				lightComp.light.SetShadowMapAsDepth(0);
			}

			myFrameBufferData.Projection = lightComp.light.GetLightBufferData().LightProj;
			lightComp.light.SetLightView(aLightList[i - 1].GetComponent<TransformComponent>().GetTransform());
			myFrameBufferData.View = lightComp.light.GetLightBufferData().LightView;
			myFrameBufferData.View = Matrix4::GetFastInverse(aLightList[i - 1].GetComponent<TransformComponent>().GetTransform());
			myFrameBufferData.CamTranslation = aLightList[i - 1].GetComponent<TransformComponent>().position;
			myFrameBufferData.RenderMode = static_cast<unsigned int>(0);

			mySceneLightBufferData.DirectionalLight.CastShadows = false;
		}

		for (int j = 0; j < timesToDraw; j++)
		{
			if (j > 0)
			{
				auto& lightComp = aLightList[i - 1].GetComponent<LightComponent>();
				lightComp.light.SetShadowMapAsDepth(j);
			}

			HRESULT result = S_FALSE;

			mySceneLightBufferData.numLights = 0;
			ZeroMemory(mySceneLightBufferData.Lights, sizeof(Light::LightBufferData) * MAX_FORWARD_LIGHTS);

			for (size_t l = 0; l < aLightList.size() && l < MAX_FORWARD_LIGHTS; l++)
			{
				auto transform = aLightList[l].GetComponent<TransformComponent>();

				mySceneLightBufferData.Lights[l] = aLightList[l].GetComponent<LightComponent>().light.GetLightBufferData();
				mySceneLightBufferData.Lights[l].Position = transform.position;
				auto rot = aLightList[l].GetComponent<TransformComponent>().rotation;
				auto direction = Matrix4::Direction(rot);
				mySceneLightBufferData.Lights[l].Direction = {
					direction.x,
					direction.y,
					direction.z
				};
				mySceneLightBufferData.Lights[l].LightView = Matrix4::GetFastInverse(transform.GetTransform());
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

			D3D11_MAPPED_SUBRESOURCE frameBufferData;

			result = DX11::myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &frameBufferData);
			if (FAILED(result))
			{
				return;
			}
			memcpy(frameBufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

			DX11::myContext->Unmap(myFrameBuffer.Get(), 0);
			DX11::myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

			DX11::myContext->PSSetShader(nullptr, nullptr, 0);
			DX11::myContext->GSSetShader(nullptr, nullptr, 0);

			for (Entity& ent : aModelList)
			{
				auto& mdlInst = ent.GetComponent<ModelComponent>().modelInstance;
				auto& mdlTransform = ent.GetComponent<TransformComponent>();
				if (!mdlInst.GetModel()) { continue; }

				D3D11_MAPPED_SUBRESOURCE objBufferData;
				ZeroMemory(&objBufferData, sizeof(objBufferData));

				ZeroMemory(&myObjectBufferData.HasBones, 16);

				myObjectBufferData.World = mdlTransform.GetTransform();
				myObjectBufferData.HasBones = mdlInst.GetModel()->HasBones();
				myObjectBufferData.IsInstanced = mdlInst.HasRenderedInstances();
				if (myObjectBufferData.HasBones)
				{
					memcpy_s(&myObjectBufferData.BoneData[0], sizeof(Matrix4x4<float>) * 128,
						&mdlInst.GetModel()->GetBoneTransforms()[0], sizeof(Matrix4x4<float>) * 128);
				}

				result = DX11::myContext->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &objBufferData);
				if (FAILED(result))
				{
					return;
				}
				memcpy(objBufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));

				DX11::myContext->Unmap(myObjectBuffer.Get(), 0);

				for (uint32_t m = 0; m < mdlInst.GetNumMeshes(); m++)
				{
					const auto& mdlData = mdlInst.GetModelData((uint16_t)m);

					DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

					DX11::myContext->IASetVertexBuffers(0, 1, mdlData.myVertexBuffer.GetAddressOf(), &mdlData.myStride, &mdlData.myOffset);
					DX11::myContext->IASetIndexBuffer(mdlData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
					DX11::myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(mdlData.myPrimitiveTopology));
					DX11::myContext->IASetInputLayout(mdlData.myInputLayout.Get());

					DX11::myContext->VSSetShader(mdlData.myVS.Get(), nullptr, 0);

					mdlData.myMaterial->SetAsResource(myMaterialBuffer);

					DX11::myContext->PSSetShader(nullptr, nullptr, 0);

					if (ent.GetComponent<ModelComponent>().modelInstance.HasRenderedInstances())
					{
						ID3D11Buffer* buffers[2] = { mdlData.myVertexBuffer.Get(), mdlInst.GetInstanceBuffer().Get() };
						UINT stride[2] = { mdlData.myStride, sizeof(ModelInstance::RenderedInstanceData) };
						UINT offset[2] = { 0, 0 };

						DX11::myContext->IASetVertexBuffers(0, 2, buffers, stride, offset);
						DX11::myContext->DrawIndexedInstanced(mdlData.myIndexCount, mdlInst.GetNumOfInstances(), 0, 0, 0);
					}
					else
					{
						DX11::myContext->IASetVertexBuffers(0, 1, mdlData.myVertexBuffer.GetAddressOf(), &mdlData.myStride, &mdlData.myOffset);
						DX11::myContext->DrawIndexed(mdlData.myIndexCount, 0, 0);
					}
				}
			}
		}
	}
}
