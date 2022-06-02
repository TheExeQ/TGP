#pragma once
#include <unordered_map>
#include <memory>
#include "Texture.h"

class TextureAssetHandler
{
public:
	static std::shared_ptr<Texture> GetTexture(const std::string& aTextureName);
	static bool LoadTexture(const std::string& aFileName);

private:
	static std::unordered_map<std::string, std::shared_ptr<Texture>> myRegistry;
};