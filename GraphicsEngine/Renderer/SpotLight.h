#pragma once
#include "Light.h"

class SpotLight : public Light
{
public:
	SpotLight() = default;
	virtual ~SpotLight() = default;
	void SetRange(float aRange);
	void SetInnerCone(float aRadius);
	void SetOuterCone(float aRadius);

	inline float GetRange() const
	{
		return ourlightBuffer.Range;
	}

	inline float GetInnerCone() const
	{
		return ourlightBuffer.SpotInnerRadius;
	}

	inline float GetOuterCone() const
	{
		return ourlightBuffer.SpotOuterRadius;
	}
};