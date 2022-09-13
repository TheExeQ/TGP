#include "ShadowRenderer.h"
#include "Core/DX11.h"
#include "Scene/Components.h"
#include "Material.h"

bool ShadowRenderer::Init()
{

}

void ShadowRenderer::Render(std::vector<Entity>& aLight, Ref<DirectionalLight> aDirectionalLight, std::vector<Entity>& aModelList)
{
	HRESULT result = S_FALSE;

	//const Light::LightBufferData lightData = aLight.GetComponent<LightComponent>().light.GetLightBufferData();
	//const auto lightTransformComp = aLight.GetComponent<TransformComponent>();

	myFrameBufferData.View = aDirectionalLight->GetLightBufferData().LightView;
	myFrameBufferData.CamTranslation = Vector3f(0.f, 0.f, 0.f);
	myFrameBufferData.Projection = aDirectionalLight->GetLightBufferData().LightProj;
	myFrameBufferData.RenderMode = static_cast<unsigned int>(0);

	D3D11_MAPPED_SUBRESOURCE frameBufferData;

	result = DX11::myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &frameBufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(frameBufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));

	DX11::myContext->Unmap(myFrameBuffer.Get(), 0);

	DX11::myContext->PSSetShader(nullptr, nullptr, 0);
	DX11::myContext->GSSetShader(nullptr, nullptr, 0);

	for (Entity& ent : aModelList)
	{
		auto& mdlInst = ent.GetComponent<ModelComponent>().modelInstance;
		auto& mdlTransform = ent.GetComponent<TransformComponent>();

		D3D11_MAPPED_SUBRESOURCE objBufferData;
		ZeroMemory(&objBufferData, sizeof(objBufferData));

		ZeroMemory(&myObjectBufferData.HasBones, 16);

		myObjectBufferData.World = mdlTransform.GetTransform();
		myObjectBufferData.HasBones = mdlInst.GetModel()->HasBones();
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
			const auto& mdlData = mdlInst.GetModelData(m);

			DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			mdlData.myMaterial->SetAsResource(myMaterialBuffer);

			// Config input assembler

			DX11::myContext->PSSetShader(nullptr, nullptr, 0);
			DX11::myContext->DrawIndexed(mdlData.myIndexCount, 0, 0);
		}
	}
}
