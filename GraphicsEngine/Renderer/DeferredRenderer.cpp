#include "DeferredRenderer.h"
#include "Core/GraphicsEngine.h"
#include "Core/DX11.h"
#include "ModelInstance.h"
#include "Material.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "EnvironmentLight.h"
#include "Math/Matrix4x4.hpp"
#include <fstream>

bool DeferredRenderer::Init()
{
    HRESULT result = S_FALSE;

    std::ifstream vsFile;
    std::ifstream psFile;

	psFile.open("../Assets/Shaders/GBufferPS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	result = DX11::myDevice->CreatePixelShader(psData.data(), psData.size(), 0, &myGBufferPS);
	psFile.close();
    
    // ----
    
    psFile.open("../Assets/Shaders/EnvironmentPS.cso", std::ios::binary);
    psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

    result = DX11::myDevice->CreatePixelShader(psData.data(), psData.size(), 0, &myEnvironmentPS);
    psFile.close();

    // ----

    vsFile.open("../Assets/Shaders/FullscreenVS.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

    result = DX11::myDevice->CreateVertexShader(vsData.data(), vsData.size(), 0, &myFullscreenVS);
    vsFile.close();

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

void DeferredRenderer::GenereteGBuffer(Entity aCamera, std::vector<Entity>& aModelList, float aDeltaTime, float aTotalTime)
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

        for (unsigned int m = 0; m < modelRef->GetNumMeshes(); ++m)
        {
            const Model::ModelData& meshData = modelRef->GetModelData(m);

            if (meshData.myMaterial)
            {
                meshData.myMaterial->SetAsResource(myMaterialBuffer);
            }
            else
            {
                // Set Default Material
            }

            D3D11_MAPPED_SUBRESOURCE matBufferData;
            ZeroMemory(&matBufferData, sizeof(matBufferData));

            myMaterialBufferData.Albedo = meshData.myMaterial->GetAlbedo();

            result = DX11::myContext->Map(myMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &matBufferData);
            if (FAILED(result))
            {
                return;
            }
            memcpy(matBufferData.pData, &myMaterialBufferData, sizeof(MaterialBufferData));

            DX11::myContext->Unmap(myMaterialBuffer.Get(), 0);

            DX11::myContext->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

            DX11::myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
            DX11::myContext->IASetInputLayout(meshData.myInputLayout.Get());

            DX11::myContext->VSSetShader(meshData.myVS.Get(), nullptr, 0);
            DX11::myContext->GSSetShader(nullptr, nullptr, 0);
            DX11::myContext->PSSetShader(myGBufferPS.Get(), nullptr, 0);

            DX11::myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
            DX11::myContext->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
            DX11::myContext->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

            if (model.GetComponent<ModelComponent>().modelInstance.HasRenderedInstances())
            {
                const auto& mdlInst = model.GetComponent<ModelComponent>().modelInstance;
				ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), mdlInst.GetInstanceBuffer().Get() };
				UINT stride[2] = { meshData.myStride, sizeof(ModelInstance::RenderedInstanceData) };
				UINT offset[2] = { 0, 0 };

				DX11::myContext->IASetVertexBuffers(0, 2, buffers, stride, offset);
                DX11::myContext->DrawIndexedInstanced(meshData.myIndexCount, mdlInst.GetNumOfInstances(), 0, 0, 0);
            }
            else
            {
				DX11::myContext->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
				DX11::myContext->DrawIndexed(meshData.myIndexCount, 0, 0);
            }
        }
    }
}

void DeferredRenderer::Render(Entity aCamera, std::vector<Entity>& aLightList, const Ref<DirectionalLight>& aDirectionalLight, const Ref<EnvironmentLight>& aEnvironmentLight, float aDeltaTime, float aTotalTime)
{
    if (aDirectionalLight)
    {
        mySceneLightBufferData.DirectionalLight = aDirectionalLight->GetLightBufferData();
    }
    if (aEnvironmentLight)
    {
        aEnvironmentLight->SetAsResource(nullptr);
    }

    mySceneLightBufferData.numLights = 0;
    ZeroMemory(mySceneLightBufferData.Lights, sizeof(Light::LightBufferData) * MAX_FORWARD_LIGHTS);

    for (size_t l = 0; l < aLightList.size() && l < MAX_FORWARD_LIGHTS; l++)
    {
        mySceneLightBufferData.Lights[l] = aLightList[l].GetComponent<LightComponent>().light.GetLightBufferData();
        mySceneLightBufferData.Lights[l].Position = aLightList[l].GetComponent<TransformComponent>().position;
        auto rot = aLightList[l].GetComponent<TransformComponent>().rotation;
        auto direction = Matrix4::Direction(rot);
        mySceneLightBufferData.Lights[l].Direction = { 
            direction.x,
            direction.y,
            direction.z
        };
        mySceneLightBufferData.numLights++;
    }

    D3D11_MAPPED_SUBRESOURCE lightBufferData;
    auto result = DX11::myContext->Map(mySceneLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightBufferData);
    if (FAILED(result))
    {
        return;
    }
    memcpy(lightBufferData.pData, &mySceneLightBufferData, sizeof(SceneLightBufferData));
    DX11::myContext->Unmap(mySceneLightBuffer.Get(), 0);

    DX11::myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DX11::myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
    DX11::myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    DX11::myContext->IASetInputLayout(nullptr);
	DX11::myContext->GSSetShader(nullptr, nullptr, 0);
	DX11::myContext->VSSetShader(myFullscreenVS.Get(), nullptr, 0);
	DX11::myContext->PSSetShader(myEnvironmentPS.Get(), nullptr, 0);
    DX11::myContext->PSSetConstantBuffers(3, 1, mySceneLightBuffer.GetAddressOf());

    DX11::myContext->Draw(3, 0);
}
