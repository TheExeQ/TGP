#pragma once
#include "Light.h"
#include "Texture.h"

class EnvironmentLight : public Light
{
public:
	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

private:
	friend class LightAssetHandler;
	
	std::shared_ptr<Texture> myTexture;
};