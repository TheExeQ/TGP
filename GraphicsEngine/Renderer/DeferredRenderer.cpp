#include "DeferredRenderer.h"
#include "Core/DX11.h"
#include "ModelInstance.h"
#include "Material.h"
#include <fstream>

DeferredRenderer::DeferredRenderer()
{
    std::ifstream psFile;

    psFile.open("../Assets/Shaders/GBufferPS.cso", std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

    DX11::myDevice->CreatePixelShader(psData.data(), psData.size(), 0, &myGBufferPS);
    psFile.close();
}

void DeferredRenderer::GenereteGBuffer(const std::shared_ptr<Camera>& aCaemra, const std::vector<std::shared_ptr<ModelInstance>>& aModelList, float aDeltaTime, float aTotalTime)
{
    HRESULT result = S_FALSE;
    D3D11_MAPPED_SUBRESOURCE bufferData;

    DX11::myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
    DX11::myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

    for (const std::shared_ptr<ModelInstance>& model : aModelList)
    {
        for (unsigned int m = 0; m > model->GetNumMeshes(); m++)
        {
            const Model::ModelData& meshData = model->GetModelData(m);

            DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
            DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());


            meshData.myMaterial->SetAsResource(myMaterialBuffer);
            DX11::myContext->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

            DX11::myContext->PSSetShader(myGBufferPS.Get(), nullptr, 0);
            DX11::myContext->DrawIndexed(meshData.myIndexCount, 0, 0);
        }

    }
}
