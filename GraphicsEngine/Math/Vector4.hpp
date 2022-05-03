#pragma once
#include <cassert>
#include <cmath>

namespace CommonUtilities
{
	template<class T>
	class Vector4
	{
	public:
		T x, y, z, w;

		//Creates a null-vector
		Vector4<T>();

		//Creates a vector (aX, aY, aZ)
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);

		//Copy constructor (compiler generated)
		Vector4<T>(const Vector4<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

		//Destructor (compiler generated)
		~Vector4<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector4<T> GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector4<T>& aVector) const;
	};

	template<class T>
	CommonUtilities::Vector4<T>::Vector4()
	{
		x = T();
		y = T();
		z = T();
		w = T();
	}

	template<class T>
	CommonUtilities::Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}

	template<class T>
	T CommonUtilities::Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	template<class T>
	void CommonUtilities::Vector4<T>::Normalize()
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
			z = z * temp;
			w = w * temp;
		}
	}

	template<class T>
	CommonUtilities::Vector4<T> CommonUtilities::Vector4<T>::GetNormalized() const
	{
		Vector4 tempVector4 = { x, y, z, w };
		tempVector4.Normalize();

		return tempVector4;
	}

	template<class T>
	T CommonUtilities::Vector4<T>::Length() const
	{
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	template<class T>
	T CommonUtilities::Vector4<T>::LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T> Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1) { return { aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w }; }

	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1) { return { aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w }; }

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar) { return { aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar }; }

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector) { return { aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar }; }

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T> Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar) 
	{ 
		if (aScalar == 0)
		{
			assert(L"Error! Scalar is zero");
		}
		return { aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar, aVector.w / aScalar }; 
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1) { aVector0 = { aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w }; }

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1) { aVector0 = { aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w }; }

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector4<T>& aVector, const T& aScalar) { aVector = { aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar }; }

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector4<T>& aVector, const T& aScalar) { aVector = { aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar, aVector.w / aScalar }; }
}