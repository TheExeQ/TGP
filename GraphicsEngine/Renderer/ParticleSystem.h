#pragma once
#include "ParticleEmitter.h"

struct ParticleSystemEmitter
{
	ParticleEmitter Emitter;
	EmitterSettingsData Settings;
};

class ParticleSystem
{
public:
	void Update(float aDeltaTime);
	FORCEINLINE const std::vector<ParticleEmitter>& GetEmitters() const { return myEmitters; };

private:
	friend class ParticleAssetHandler;
	std::vector<ParticleEmitter> myEmitters;
};

