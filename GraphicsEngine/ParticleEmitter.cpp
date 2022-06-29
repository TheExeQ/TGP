#include "ParticleEmitter.h"
#include <filesystem>
#include <fstream>
#include "TextureAssetHandler.h"
#include "CU/Timer.hpp"
#include "DX11.h"
#include "Random.h"

bool ParticleEmitter::Init(const ParticleEmitterTemplate& aTemplate)
{
	myEmitterSettings = aTemplate.EmitterSettings;
	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	const size_t maxNumberOfParticles = static_cast<size_t>(ceilf(myEmitterSettings.SpawnRate * myEmitterSettings.LifeTime));
	myParticles.resize(maxNumberOfParticles);
	
	for (size_t p = 0; p < myParticles.size(); p++)
	{
		InitParticle(p);
	}

	HRESULT result = S_FALSE;

	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(myParticles.size()) * static_cast<UINT>(sizeof(ParticleVertex));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = &myParticles[0];
	result = DX11::myDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &myVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::ifstream vsFile;

	vsFile.open("../Assets/Shaders/ParticleVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	result = DX11::myDevice->CreateVertexShader(vsData.data(), vsData.size(), 0, &myVertexShader);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();

	std::ifstream gsFile;

	gsFile.open("../Assets/Shaders/ParticleGS.cso", std::ios::binary);
	std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };

	result = DX11::myDevice->CreateGeometryShader(gsData.data(), gsData.size(), 0, &myGeometryShader);
	if (FAILED(result))
	{
		return false;
	}
	gsFile.close();

	std::ifstream psFile;

	psFile.open("../Assets/Shaders/ParticlePS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	result = DX11::myDevice->CreatePixelShader(psData.data(), psData.size(), 0, &myPixelShader);
	if (FAILED(result))
	{
		return false;
	}
	psFile.close();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SCALE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	result = DX11::myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &myInputLayout);
	if (FAILED(result))
	{
		return false;
	}

	if (!TextureAssetHandler::LoadTexture("ParticleStar.dds"))
	{
		return false;
	}

	myTexture = TextureAssetHandler::GetTexture("ParticleStar.dds");

	myStride = sizeof(ParticleVertex);
	myOffset = 0;
	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	return true;
}

void ParticleEmitter::Update(float aDeltaTime)
{
	for (size_t p = 0; p < myParticles.size(); p++)
	{
		ParticleVertex& particle = myParticles[p];
		particle.LifeTime += aDeltaTime;

		if (particle.LifeTime >= myEmitterSettings.LifeTime)
		{
			InitParticle(p);
		}
		
		particle.Color.x = Random::GetRandomFloat(0, 1);
		particle.Color.y = Random::GetRandomFloat(0, 1);
		particle.Color.z = Random::GetRandomFloat(0, 1);
		particle.Color.w = std::clamp(myEmitterSettings.LifeTime - (myEmitterSettings.LifeTime * particle.LifeTime), 0.0f, 1.0f);

		particle.Position += Vector4f(particle.Velocity.x, particle.Velocity.y, particle.Velocity.z, 0) * aDeltaTime;
		particle.Velocity.y -= 9.81f;
	}
}

void ParticleEmitter::SetAsResource() const
{
	HRESULT result = S_FALSE;

	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::myContext->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

	if (FAILED(result))
	{
		return;
	}
	
	memcpy_s(bufferData.pData, sizeof(ParticleVertex) * myParticles.size(), &myParticles[0], sizeof(ParticleVertex) * myParticles.size());

	DX11::myContext->Unmap(myVertexBuffer.Get(), 0);

	DX11::myContext->VSSetShader(myVertexShader.Get(), nullptr, 0);
	DX11::myContext->GSSetShader(myGeometryShader.Get(), nullptr, 0);
	DX11::myContext->PSSetShader(myPixelShader.Get(), nullptr, 0);

	if (myTexture)
	{
		myTexture->SetAsResource(0);
	}
}

void ParticleEmitter::Draw() const
{
	DX11::myContext->Draw(static_cast<UINT>(myParticles.size()), 0);
}

void ParticleEmitter::InitParticle(size_t aParticleIndex)
{
	myParticles[aParticleIndex].Position = { 0,0,0,1 };
	myParticles[aParticleIndex].Color = myEmitterSettings.StartColor;

	float randomFloat = Random::GetRandomFloat(-2.f, 2.f);

	myParticles[aParticleIndex].Velocity = Vector3f(myEmitterSettings.StartVelocity.x * randomFloat,
		myEmitterSettings.StartVelocity.y, myEmitterSettings.StartVelocity.z);

	myParticles[aParticleIndex].Scale = Vector3f(1.0f, 1.0f, 1.0f) * myEmitterSettings.StartSize;
	myParticles[aParticleIndex].LifeTime = Random::GetRandomFloat(0.f, 2.5f);
}
