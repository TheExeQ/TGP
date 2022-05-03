#pragma once
#include "Vector4.hpp"
#include <array>
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();

		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);

		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);

	private:
		std::array<std::array<T, 4>, 4> myMatrix;
	};

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4()
	{
		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				myMatrix[row][column] = (row == column) ? static_cast<T>(1) : static_cast<T>(0);
			}
		}
	}

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		myMatrix = aMatrix.myMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> rotationMatrix;
		rotationMatrix.myMatrix[1][1] = std::cos(aAngleInRadians);
		rotationMatrix.myMatrix[1][2] = std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][1] = -std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][2] = std::cos(aAngleInRadians);
		return rotationMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> rotationMatrix;
		rotationMatrix.myMatrix[0][0] = std::cos(aAngleInRadians);
		rotationMatrix.myMatrix[0][2] = -std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][0] = std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][2] = std::cos(aAngleInRadians);
		return rotationMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> rotationMatrix;
		rotationMatrix.myMatrix[0][0] = std::cos(aAngleInRadians);
		rotationMatrix.myMatrix[0][1] = std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[1][0] = -std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[1][1] = std::cos(aAngleInRadians);
		return rotationMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> tempMatrix;
		for (short row = 1; row < 5; row++)
		{
			for (short column = 1; column < 5; column++)
			{
				tempMatrix(row, column) = aMatrixToTranspose(column, row);
			}
		}
		return tempMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> tempMatrix;
		Matrix4x4<T> rotaionMatrix;
		for (short row = 1; row < 4; row++)
		{
			for (short column = 1; column < 4; column++)
			{
				rotaionMatrix(row, column) = aTransform(row, column);
			}
		}
		rotaionMatrix = Matrix4x4<T>::Transpose(rotaionMatrix);
		for (short column = 1; column < 4; column++)
		{
			tempMatrix(4, column) = aTransform(4, column) * -1;
		}

		return tempMatrix * rotaionMatrix;
	}

	template<typename T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert((aRow > 0 && aRow < 5 && aColumn > 0 && aColumn < 5) && "Row or Column is not a valid index.");
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert((aRow > 0 && aRow < 5 && aColumn > 0 && aColumn < 5) && "Row or Column is not a valid index.");
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline bool operator==(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (short row = 1; row < 5; row++)
		{
			for (short column = 1; column < 5; column++)
			{
				if (aMatrix0(row, column) != aMatrix1(row, column))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<typename T>
	inline Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> finalVector;

		finalVector.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		finalVector.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		finalVector.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		finalVector.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);

		return finalVector;
	}

	template<typename T>
	inline void operator*=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		aMatrix0 = aMatrix0 * aMatrix1;
	}

	template<typename T>
	inline Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> tempMatrix;
		for (short i = 1; i < 5; i++)
		{
			for (short row = 1; row < 5; row++)
			{
				if (tempMatrix(i, row) != 0)
				{
					tempMatrix(i, row) = 0;
				}
				for (short column = 1; column < 5; column++)
				{
					tempMatrix(i, row) += aMatrix0(i, column) * aMatrix1(column, row);
				}
			}
		}
		return tempMatrix;
	}

	template<typename T>
	inline void operator-=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (short row = 1; row < 5; row++)
		{
			for (short column = 1; column < 5; column++)
			{
				aMatrix0(row, column) -= aMatrix1(row, column);
			}
		}
	}

	template<typename T>
	inline Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> finalMatrix(aMatrix0);
		finalMatrix -= aMatrix1;
		return finalMatrix;
	}

	template<typename T>
	inline void operator+=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (short row = 1; row < 5; row++)
		{
			for (short column = 1; column < 5; column++)
			{
				aMatrix0(row, column) += aMatrix1(row, column);
			}
		}
	}

	template<typename T>
	inline Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> finalMatrix(aMatrix0);
		finalMatrix += aMatrix1;
		return finalMatrix;
	}
}