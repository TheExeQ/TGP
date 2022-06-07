#pragma once
#include <string>
#include <array>
#include <memory>
#include "Math/Vector3.hpp"
#include "Texture.h"

namespace MaterialTextureChannel
{
	enum
	{
		Albedo,
		Normal,
		COUNT
	};
}

class Material
{
public:
	Material();
	virtual ~Material() = default;

	void Init(std::string aName, const CommonUtilities::Vector3<float>& anAlbedo);

	_inline const std::string& GetName() const { return myName; };
	_inline const CommonUtilities::Vector3<float>& GetAlbedo() const { return myMaterialData.Albedo; };

	void SetAlbedoTexture(std::shared_ptr<Texture> aTexture);
	void SetNormalTexture(std::shared_ptr<Texture> aTexture);
	
	void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer) const;

	struct MaterialData
	{
		CommonUtilities::Vector3<float> Albedo;
		float Padding;
	};
	
private:
	std::string myName;
	MaterialData myMaterialData;

	std::array<std::shared_ptr<Texture>, MaterialTextureChannel::COUNT> myTextures;
};