#pragma once

#include <cassert>
#include <cmath>

#define PI 3.1415926535f

namespace CommonUtilities
{
	template<class T>
	class Vector2
	{
	public:
		T x, y;
	
		//Creates a null-vector
		Vector2<T>();

		//Creates a vector (aX, aY, aZ)
		Vector2<T>(const T& aX, const T& aY);

		//Copy constructor (compiler generated)
		Vector2<T>(const Vector2<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector2<T>& operator=(const Vector2<T>&aVector2) = default;

		//Destructor (compiler generated)
		~Vector2<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector2<T> GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>&aVector) const;

		static Vector2<T> Up();
		static Vector2<T> Down();
		static Vector2<T> Left();
		static Vector2<T> Right();

		static Vector2<T> Up(T aRadians);
		static Vector2<T> Down(T aRadians);
		static Vector2<T> Left(T aRadians);
		static Vector2<T> Right(T aRadians);
	};

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Right(T aRadians)
	{
		return { std::cosf(aRadians), std::sinf(aRadians) };
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Left(T aRadians)
	{
		return { std::cosf(aRadians - PI), std::sinf(aRadians - PI) };
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Down(T aRadians)
	{
		return { std::cosf(aRadians + PI * 0.5f), std::sinf(aRadians + PI * 0.5f) };
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Up(T aRadians)
	{
		return { std::cosf(aRadians - PI * 0.5f), std::sinf(aRadians - PI * 0.5f) };
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Right()
	{
		return { 1, 0 };
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Left()
	{
		return { -1, 0 };
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Down()
	{
		return { 0, 1 };
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::Up()
	{
		return { 0, -1 };
	}

	template<class T>
	CommonUtilities::Vector2<T>::Vector2()
	{
		x = T();
		y = T();
	}

	template<class T>
	CommonUtilities::Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	template<class T>
	T CommonUtilities::Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}

	template<class T>
	void CommonUtilities::Vector2<T>::Normalize()	
	{
		if (Length() == 0)
		{
			assert(L"Error! length is zero");
		}
		else
		{
			T temp = 1 / Length();
			x = x * temp;
			y = y * temp;
		}
	}

	template<class T>
	CommonUtilities::Vector2<T> CommonUtilities::Vector2<T>::GetNormalized() const
	{
		Vector2 tempVector2 = {x, y};
		tempVector2.Normalize();

		return tempVector2;
	}

	template<class T>
	T CommonUtilities::Vector2<T>::Length() const
	{
		return std::sqrt(x * x + y * y);
	}

	template<class T>
	T CommonUtilities::Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T> Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1) { return {aVector0.x + aVector1.x, aVector0.y + aVector1.y }; }

	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1) { return { aVector0.x - aVector1.x, aVector0.y - aVector1.y }; }

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar) { return { aVector.x * aScalar, aVector.y * aScalar }; }

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector) { return { aVector.x * aScalar, aVector.y * aScalar }; }

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T> Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar) 
	{
		if (aScalar == 0)
		{
			assert(L"Error! Scalar is zero");
		}
		return { aVector.x / aScalar, aVector.y / aScalar }; 
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1) { aVector0 = { aVector0.x + aVector1.x, aVector0.y + aVector1.y }; }

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1) { aVector0 = { aVector0.x - aVector1.x, aVector0.y - aVector1.y }; }

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector2<T>& aVector, const T& aScalar) { aVector = { aVector.x * aScalar, aVector.y * aScalar }; }

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector2<T>& aVector, const T& aScalar) { aVector = { aVector.x / aScalar, aVector.y / aScalar }; }
}