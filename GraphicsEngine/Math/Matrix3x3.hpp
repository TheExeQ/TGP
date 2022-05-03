#pragma once
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include <array>
#include <math.h>
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix.
		Matrix3x3<T>();

		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);

		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

	private:
		std::array<std::array<T, 3>, 3> myMatrix;
	};

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				myMatrix[row][column] = (row == column) ? 1 : 0;
			}
		}
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		myMatrix = aMatrix.myMatrix;
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (short row = 0; row < 3; row++)
		{
			for (short column = 0; column < 3; column++)
			{
				myMatrix[row][column] = aMatrix(row + 1, column + 1);
			}
		}
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3<T> rotationMatrix;
		rotationMatrix.myMatrix[1][1] = std::cos(aAngleInRadians);
		rotationMatrix.myMatrix[1][2] = std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][1] = -std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][2] = std::cos(aAngleInRadians);
		return rotationMatrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3<T> rotationMatrix;
		rotationMatrix.myMatrix[0][0] = std::cos(aAngleInRadians);
		rotationMatrix.myMatrix[0][2] = -std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][0] = std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[2][2] = std::cos(aAngleInRadians);
		return rotationMatrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3<T> rotationMatrix;
		rotationMatrix.myMatrix[0][0] = std::cos(aAngleInRadians);
		rotationMatrix.myMatrix[0][1] = std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[1][0] = -std::sin(aAngleInRadians);
		rotationMatrix.myMatrix[1][1] = std::cos(aAngleInRadians);
		return rotationMatrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> tempMatrix;
		for (short row = 1; row < 4; row++)
		{
			for (short column = 1; column < 4; column++)
			{
				tempMatrix(row, column) = aMatrixToTranspose(column, row);
			}
		}
		return tempMatrix;
	}

	template<typename T>
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		assert((aRow > 0 && aRow < 4 && aColumn > 0 && aColumn < 4) && "Row or Column is not a valid index.");
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		assert((aRow > 0 && aRow < 4 && aColumn > 0 && aColumn < 4) && "Row or Column is not a valid index.");
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> finalMatrix(aMatrix0);
		finalMatrix += aMatrix1;
		return finalMatrix;
	}

	template<typename T>
	inline void operator+=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (short row = 1; row < 4; row++)
		{
			for (short column = 1; column < 4; column++)
			{
				aMatrix0(row, column) += aMatrix1(row, column);
			}
		}
	}

	template<typename T>
	inline Matrix3x3<T> operator-(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> finalMatrix(aMatrix0);
		finalMatrix -= aMatrix1;
		return finalMatrix;
	}

	template<typename T>
	inline void operator-=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (short row = 1; row < 4; row++)
		{
			for (short column = 1; column < 4; column++)
			{
				aMatrix0(row, column) -= aMatrix1(row, column);
			}
		}
	}

	template<typename T>
	inline Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;
		for (short i = 1; i < 4; i++)
		{
			for (short row = 1; row < 4; row++)
			{
				if (tempMatrix(i, row) != 0)
				{
					tempMatrix(i, row) = 0;
				}
				for (short column = 1; column < 4; column++)
				{
					tempMatrix(i, row) += aMatrix0(i, column) * aMatrix1(column, row);
				}
			}
		}
		return tempMatrix;
	}

	template<typename T>
	inline void operator*=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		aMatrix0 = aMatrix0 * aMatrix1;
	}

	template<typename T>
	inline Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Vector3<T> finalVector;

		finalVector.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		finalVector.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		finalVector.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return finalVector;
	}

	template<typename T>
	inline bool operator==(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (short row = 1; row < 4; row++)
		{
			for (short column = 1; column < 4; column++)
			{
				if (aMatrix0(row, column) != aMatrix1(row, column))
				{
					return false;
				}
			}
		}
		return true;
	}
}