#pragma once
#include <math.h>

namespace CommonUtilities
{
	template<typename T>
	class Math
	{
	public:
		inline static T ConvertToRadians(T aDegrees);
		inline static T ConvertToDegrees(T aRadians);

	private:
		inline static const float myPi = 3.1415f;
	};

	template<typename T>
	T Math<T>::ConvertToDegrees(T aRadians)
	{
		return 180 * (aRadians / myPi);
	}

	template<typename T>
	T Math<T>::ConvertToRadians(T aDegrees)
	{
		return aDegrees * (myPi / 180);
	}
}