#pragma once
#include <math.h>

namespace CommonUtilities
{
	template<typename T>
	class Math
	{
	public:
		inline static T Radians(T aDegrees);
		inline static T Degrees(T aRadians);

	private:
		inline static const float myPi = 3.1415f;
	};

	template<typename T>
	T Math<T>::Degrees(T aRadians)
	{
		return 180 * (aRadians / myPi);
	}

	template<typename T>
	T Math<T>::Radians(T aDegrees)
	{
		return aDegrees * (myPi / 180);
	}
}