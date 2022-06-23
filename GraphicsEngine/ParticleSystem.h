#pragma once
#include "Scene.h"
#include "ParticleEmitter.h"

struct ParticleSystemEmitter
{
	ParticleEmitter Emitter;
	EmitterSettingsData Settings;
	Transform Transform;
};

class ParticleSystem : public SceneObject
{
public:
	void Update(float aDeltaTime);
	FORCEINLINE const std::vector<ParticleEmitter>& GetEmitters() const { return myEmitters; };

private:
	friend class ParticleAssetHandler;
	std::vector<ParticleEmitter> myEmitters;
};

