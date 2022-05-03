#pragma once
#include "Vector2.hpp"
#include <limits>

namespace CommonUtilities
{
	template<typename T>
	class Line
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line();

		// Copy constructor.
		Line(const Line <T>& aLine);
		
		// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);	
		
		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		
		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);
		
		// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
		bool Inside(const Vector2<T>&aPosition) const;
		
		// Returns the direction of the line.
		const Vector2<T> GetDirection() const;
		
		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T> GetNormal() const;

	private:
		Vector2<T> myFirstPoint;
		Vector2<T> mySecondPoint;
	};

	template<typename T>
	const Vector2<T> CommonUtilities::Line<T>::GetNormal() const
	{
		Vector2<T> tempVect = { -GetDirection().y, GetDirection().x };
		return tempVect.GetNormalized();
	}

	template<typename T>
	const Vector2<T> CommonUtilities::Line<T>::GetDirection() const
	{
		return { mySecondPoint.x - myFirstPoint.x, mySecondPoint.y - myFirstPoint.y};
	}

	template<typename T>
	bool CommonUtilities::Line<T>::Inside(const Vector2<T>& aPosition) const
	{
		T temp = aPosition.Dot(GetNormal()) - myFirstPoint.Dot(GetNormal());
		if (temp - std::numeric_limits<T>::epsilon() * 2 <= 0)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	void CommonUtilities::Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myFirstPoint = aPoint;
		mySecondPoint = aPoint + aDirection;
	}

	template<typename T>
	void CommonUtilities::Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myFirstPoint = aPoint0;
		mySecondPoint = aPoint1;
	}

	template<typename T>
	CommonUtilities::Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myFirstPoint = aPoint0;
		mySecondPoint = aPoint1;
	}

	template<typename T>
	CommonUtilities::Line<T>::Line(const Line <T>& aLine)
	{
		myFirstPoint = aLine.myFirstPoint;
		mySecondPoint = aLine.mySecondPoint;
	}

	template<typename T>
	CommonUtilities::Line<T>::Line()
	{
		myFirstPoint = { 0, 0 };
		mySecondPoint = { 0, 0 };
	}

}

