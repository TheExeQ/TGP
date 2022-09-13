#pragma once
#include "Core/Base.h"
#include "Texture.h"
#include "DepthStencil.h"
#include "GBuffer.h"

#include <memory>
#include <unordered_map>

class TextureAssetHandler
{
public:
	static Ref<Texture> GetTexture(const std::string& aTextureName);
	static bool LoadTexture(const std::string& aFileName);
	static Scope<GBuffer> CreateGBuffer(int aWidth, int aHeight);
	static Ref<DepthStencil> CreateDepthStencil(const std::string aName, size_t aWidth, size_t aHeight);

private:
	static std::unordered_map<std::string, Ref<Texture>> myRegistry;
};