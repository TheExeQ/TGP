#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Plane
	{
	public:
		// Default constructor.
		Plane();

		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2 -aPoint0).
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		// Constructor taking a point and a normal.
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);
		
		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		
		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		
		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool Inside(const Vector3<T>&aPosition);
		
		// Returns the normal of the plane.
		const Vector3<T> GetNormal();

		Vector3<T> GetPosition();

	private:
		Vector3<T> myPoint;
		Vector3<T> myNormal;
	};

	template<typename T>
	const Vector3<T> CommonUtilities::Plane<T>::GetNormal() 
	{
		return myNormal;
	}

	template<typename T>
	Vector3<T> CommonUtilities::Plane<T>::GetPosition()
	{
		return myPoint;
	}

	template<typename T>
	bool CommonUtilities::Plane<T>::Inside(const Vector3<T>& aPosition)
	{
		T temp = aPosition.Dot(GetNormal()) - myPoint.Dot(GetNormal());
		if (temp - std::numeric_limits<T>::epsilon() * 2 <= 0)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	void CommonUtilities::Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal;
	}

	template<typename T>
	void CommonUtilities::Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;

		Vector3<T> tempVect1 = aPoint1 - myPoint;
		Vector3<T> tempVect2 = aPoint2 - myPoint;

		myNormal = tempVect1.Cross(tempVect2).GetNormalized();
	}

	template<typename T>
	CommonUtilities::Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal)
	{
		myPoint = aPoint0;
		myNormal = aNormal;
	}

	template<typename T>
	CommonUtilities::Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;

		Vector3<T> tempVect1 = aPoint1 - myPoint;
		Vector3<T> tempVect2 = aPoint2 - myPoint;

		myNormal = tempVect1.Cross(tempVect2).GetNormalized();
	}

	template<typename T>
	CommonUtilities::Plane<T>::Plane()
	{
		myPoint = { 0, 0, 0};
		myNormal = { 0, 0, 0 };
	}

}

