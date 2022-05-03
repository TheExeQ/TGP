#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<class T>
	class AABB3D
	{
	public:
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D();

		// Copy constructor.
		AABB3D(const AABB3D<T>& aAABB3D);

		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T>& aPosition) const;

		Vector3<T> GetPosition() { return myPosition; };

	private:
		Vector3<T> myPosition;
		Vector3<T> myMin;
		Vector3<T> myMax;
	};

	template<class T>
	bool CommonUtilities::AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return aPosition.x >= myMin.x && aPosition.y >= myMin.y && aPosition.z >= myMin.z
			&& aPosition.x <= myMax.x && aPosition.y <= myMax.y && aPosition.z <= myMax.z;
	}

	template<class T>
	void CommonUtilities::AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMin = aMin;
		myMax = aMax;
		myPosition = (myMin + myMin) / T(2);
	}

	template<class T>
	CommonUtilities::AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMin = aMin;
		myMax = aMax;
		myPosition = (myMin + myMin) / T(2);
	}

	template<class T>
	CommonUtilities::AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D)
	{
		myMin = aAABB3D.myMin;
		myMax = aAABB3D.myMax;
		myPosition = (myMin + myMin) / T(2);
	}

	template<class T>
	CommonUtilities::AABB3D<T>::AABB3D()
	{
		myMin = { 0, 0, 0 };
		myMax = { 0, 0, 0 };
		myPosition = {0, 0, 0};
	}

}