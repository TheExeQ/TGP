#include "Material.h"

Material::Material()
{

}

void Material::Init(std::string aName, const CommonUtilities::Vector3<float>& anAlbedo)
{
	myName = aName;
	myAlbedo = anAlbedo;
}
