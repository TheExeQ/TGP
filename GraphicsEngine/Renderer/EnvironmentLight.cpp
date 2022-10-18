#include "gepch.h"
#include "EnvironmentLight.h"

void EnvironmentLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
	if (myTexture)
	{
		myTexture->SetAsResource(14);
	}
}
