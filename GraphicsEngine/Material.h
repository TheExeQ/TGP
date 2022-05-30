#pragma once
#include <string>
#include "Math/Vector3.hpp"

class Material
{
public:
	Material();
	virtual ~Material() = default;

	void Init(std::string aName, const CommonUtilities::Vector3<float>& anAlbedo);

	_inline const std::string& GetName() const { return myName; };
	_inline const CommonUtilities::Vector3<float>& GetAlbedo() const { return myAlbedo; };

private:
	std::string myName;
	CommonUtilities::Vector3<float> myAlbedo;
	
};