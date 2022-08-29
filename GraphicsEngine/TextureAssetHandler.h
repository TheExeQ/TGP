#pragma once
#include <unordered_map>
#include <memory>
#include "Texture.h"
#include "GBuffer.h"

class TextureAssetHandler
{
public:
	static std::shared_ptr<Texture> GetTexture(const std::string& aTextureName);
	static bool LoadTexture(const std::string& aFileName);
	static std::unique_ptr<GBuffer> CreateGBuffer(int aWidth, int aHeight);

private:
	static std::unordered_map<std::string, std::shared_ptr<Texture>> myRegistry;
};