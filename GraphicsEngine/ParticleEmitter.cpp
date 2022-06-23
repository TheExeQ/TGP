#include "ParticleEmitter.h"
#include "TextureAssetHandler.h"
#include "CU/Timer.hpp"
#include "DX11.h"

bool ParticleEmitter::Init(const ParticleEmitterTemplate& aTemplate)
{
	myEmitterSettings = aTemplate.EmitterSettings;

	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	const size_t maxNumberOfParticles = static_cast<size_t>(ceilf(myEmitterSettings.SpawnRate * myEmitterSettings.LifeTime));

	myParticles.resize(maxNumberOfParticles);

	if (!TextureAssetHandler::LoadTexture("ParticleStar.dds"))
	{
		return false;
	}

	myTexture = TextureAssetHandler::GetTexture("ParticleStar.dds");

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
	}
}

void ParticleEmitter::SetAsResource() const
{
	HRESULT result = S_FALSE;

	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = DX11::myContext->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

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

}
