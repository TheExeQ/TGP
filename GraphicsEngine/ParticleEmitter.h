#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <memory>
#include "ParticleEmitterTemplate.h"
#include "ParticleVertex.h"
#include "Texture.h"

using namespace Microsoft::WRL;

class ParticleEmitter
{
public:
	virtual ~ParticleEmitter() = default;

	virtual bool Init(const ParticleEmitterTemplate& aTemplate);
	virtual void Update(float aDeltaTime);

	virtual void SetAsResource() const;
	virtual void Draw() const;

	_inline const EmitterSettingsData& GetEmitterSettings() const { return myEmitterSettings; };

private:
	void InitParticle(size_t aParticleIndex);

	friend class ParticleEmitterHandler;
	friend class ForwardRenderer;

	EmitterSettingsData  myEmitterSettings;

	std::vector<ParticleVertex> myParticles;

	ComPtr<ID3D11Buffer> myVertexBuffer;
	UINT myStride;
	UINT myOffset;
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11GeometryShader> myGeometryShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	UINT myPrimitiveTopology;
	ComPtr<ID3D11InputLayout> myInputLayout;

	std::shared_ptr<Texture> myTexture;
};

