#include "ParticleAssetHandler.h"

bool ParticleAssetHandler::Init()
{
	ParticleEmitterTemplate baseEmitterTemplate;
	baseEmitterTemplate.EmitterSettings.SpawnRate = 1000.0f;
	baseEmitterTemplate.EmitterSettings.LifeTime = 5.0f;
	baseEmitterTemplate.EmitterSettings.StartColor = { 1.0f,1.0f,0.0f,1.0f };
	baseEmitterTemplate.EmitterSettings.StartSize = 5.0f;
	baseEmitterTemplate.EmitterSettings.StartVelocity = { 100.0f,500.0f,100.0f };

	ParticleSystemTemplate baseSystemTemplate;
	baseSystemTemplate.EmitterTemplates.push_back(baseEmitterTemplate);
	mySystemTemplates["Core"] = baseSystemTemplate;
	return true;
}

std::shared_ptr<ParticleSystem> ParticleAssetHandler::GetParticleSystem(const std::string& aSystemName)
{
	if (mySystemTemplates.find(aSystemName) != mySystemTemplates.end())
	{
		std::shared_ptr<ParticleSystem> system = std::make_shared<ParticleSystem>();
		for (size_t templ = 0; templ < mySystemTemplates[aSystemName].EmitterTemplates.size(); templ++)
		{
			ParticleEmitter emitter;
			emitter.Init(mySystemTemplates[aSystemName].EmitterTemplates[templ]);
			system->myEmitters.push_back(emitter);
		}
		return system;
	}
	return nullptr;
}
