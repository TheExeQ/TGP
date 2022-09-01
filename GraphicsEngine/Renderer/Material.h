#pragma once
#include "Core/Base.h"
#include "Math/Vector3.hpp"
#include "Texture.h"

#include <string>
#include <array>
#include <memory>

namespace MaterialTextureChannel
{
	enum
	{
		Albedo,
		Normal,
		Material,
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

	void SetAlbedoTexture(Ref<Texture> aTexture);
	void SetNormalTexture(Ref<Texture> aTexture);
	void SetMaterialTexture(Ref<Texture> aTexture);
	
	void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer) const;

	struct MaterialData
	{
		CommonUtilities::Vector3<float> Albedo;
		float Padding;
	};
	
private:
	std::string myName;
	MaterialData myMaterialData;

	std::array<Ref<Texture>, MaterialTextureChannel::COUNT> myTextures;
};