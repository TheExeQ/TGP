#include "Light.h"

void Light::Init(Vector3<float> aColor, float anIntensity)
{
	ourlightBuffer.Color = aColor;
	ourlightBuffer.Intensity = anIntensity;
}

