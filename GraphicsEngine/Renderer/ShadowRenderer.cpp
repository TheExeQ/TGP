#include "ShadowRenderer.h"
#include "Core/DX11.h"
#include "Scene/Components.h"
#include "Material.h"

bool ShadowRenderer::Init()
{

}

void ShadowRenderer::Render(Entity& aLight, std::vector<Entity>& aModelList)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE bufferData;

	const Light::LightBufferData lightData = aLight.GetComponent<LightComponent>().light.GetLightBufferData();
	const auto lightTransformComp = aLight.GetComponent<TransformComponent>();

	myFrameBufferData.View = lightData.LightView;
	myFrameBufferData.CamTranslation = lightTransformComp.position;
	myFrameBufferData.Projection = lightData.LightProj;
	myFrameBufferData.RenderMode = static_cast<unsigned int>(0);

	DX11::myContext->PSSetShader(nullptr, nullptr, 0);
	DX11::myContext->GSSetShader(nullptr, nullptr, 0);

	// #TODO: Continue here (Page 28)

	// Map

	for (Entity& ent : aModelList)
	{
		const auto& mdlInst = ent.GetComponent<ModelComponent>().modelInstance;

		for (uint32_t m = 0; m < mdlInst.GetNumMeshes(); m++)
		{
			const auto& mdlData = mdlInst.GetModelData(m);

			// Set Obj buffers

			DX11::myContext->VSGetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			// Config input assembler

			mdlData.myMaterial->SetAsResource(myMaterialBuffer);

			DX11::myContext->PSSetShader(nullptr, nullptr, 0);
			DX11::myContext->DrawIndexed(mdlData.myIndexCount, 0, 0);
		}
	}
}
