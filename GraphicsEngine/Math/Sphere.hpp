#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<class T>
	class Sphere
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();

		// Copy constructor.
		Sphere(const Sphere<T>& aSphere);

		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T>& aCenter, T aRadius);

		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const;

		Vector3<T> GetPosition() { return myPosition; };
		T GetRadius() { return myRadius; };

	private:
		Vector3<T> myPosition;
		T myRadius;
	};

	template<class T>
	bool CommonUtilities::Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> distance = (myPosition - aPosition);
		return distance.LengthSqr() <= myRadius * myRadius;
	}

	template<class T>
	void CommonUtilities::Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myPosition = aCenter;
		myRadius = aRadius;
	}

	template<class T>
	CommonUtilities::Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
	{
		myPosition = aCenter;
		myRadius = aRadius;
	}

	template<class T>
	CommonUtilities::Sphere<T>::Sphere(const Sphere<T>& aSphere)
	{
		myPosition = aSphere.myPosition;
		myRadius = aSphere.myRadius;
	}

	template<class T>
	CommonUtilities::Sphere<T>::Sphere()
	{
		myPosition = { 0, 0, 0 };
		myRadius = 0;
	}

}