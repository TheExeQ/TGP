#include "gepch.h"
#include "ParticleSystem.h"

void ParticleSystem::Update(float aDeltaTime)
{
	for (auto& emitter : myEmitters)
	{
		emitter.Update(aDeltaTime);
	}
}
