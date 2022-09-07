#include "SpotLight.h"

void SpotLight::SetRange(float aRange)
{
	ourlightBuffer.Range = aRange;
}

void SpotLight::SetInnerCone(float aRadius)
{
	ourlightBuffer.SpotInnerRadius = aRadius;
}

void SpotLight::SetOuterCone(float aRadius)
{
	ourlightBuffer.SpotOuterRadius = aRadius;
}
