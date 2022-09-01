#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "ParticleEmitterTemplate.h"
#include "ParticleSystem.h"

class ParticleAssetHandler
{
public:
	static bool Init();
	static Ref<ParticleSystem> GetParticleSystem(const std::string& aSystemName);

private:
	typedef std::unordered_map<std::string, ParticleEmitterTemplate> EmitterTemplateMap;
	typedef std::unordered_map<std::string, ParticleSystemTemplate> SystemTemplateMap;

	inline static EmitterTemplateMap myEmitterTemplates{};
	inline static SystemTemplateMap mySystemTemplates{};
};

