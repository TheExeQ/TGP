#include "SpotLight.h"

void SpotLight::SetRange(float aRange)
{
	ourLightBuffer.Range = aRange;
}

void SpotLight::SetInnerCone(float aRadius)
{
	ourLightBuffer.SpotInnerRadius = aRadius;
}

void SpotLight::SetOuterCone(float aRadius)
{
	ourLightBuffer.SpotOuterRadius = aRadius;
}
