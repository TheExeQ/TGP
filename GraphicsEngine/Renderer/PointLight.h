#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() = default;
	virtual ~PointLight() = default;
	void SetRange(float aRange);

	inline float GetRange() const 
	{
		return ourLightBuffer.Range;
	}
};