#pragma once

namespace CommonUtilities
{
	template <class T>
	void SelectionSort(std::vector<T>& aVector);
	template <class T>
	void BubbleSort(std::vector<T>& aVector);
	template <class T>
	void QuickSort(std::vector<T>& aVector);
	template <class T>
	void MergeSort(std::vector<T>& aVector);

	template<class T>
	void SelectionSort(std::vector<T>& aVector)
	{
		for (size_t i = 0; i < aVector.size(); i++)
		{
			size_t minIndex = i;
			for (size_t j = i + 1; j < aVector.size(); j++)
			{
				if (aVector[j] < aVector[minIndex])
				{
					minIndex = j;
				}
			}
			std::swap(aVector[i], aVector[minIndex]);
		}
	};

	template<class T>
	void BubbleSort(std::vector<T>& aVector)
	{
		for (size_t i = 0; i < aVector.size(); i++)
		{
			for (size_t j = 0; j < aVector.size() - 1; j++)
			{
				if (aVector[j] > aVector[j + 1])
				{
					std::swap(aVector[j], aVector[j + 1]);
				}
			}
		}
	};

	template<class T>
	void QuickSort(std::vector<T>& aVector)
	{
		if (aVector.size() <= 1)
		{
			return;
		}

		size_t pivotIndex = aVector.size() / 2;
		T pivotValue = aVector[pivotIndex];
		aVector.erase(aVector.begin() + pivotIndex);

		std::vector<T> leftVector;
		std::vector<T> rightVector;

		for (size_t i = 0; i < aVector.size(); i++)
		{
			if (aVector[i] < pivotValue)
			{
				leftVector.push_back(aVector[i]);
			}
			else
			{
				rightVector.push_back(aVector[i]);
			}
		}

		QuickSort(leftVector);
		QuickSort(rightVector);

		aVector.clear();

		aVector.insert(aVector.begin(), leftVector.begin(), leftVector.end());
		aVector.push_back(pivotValue);
		aVector.insert(aVector.end(), rightVector.begin(), rightVector.end());
	};

	template<class T>
	void MergeSort(std::vector<T>& aVector)
	{
		if (aVector.size() <= 1)
		{
			return;
		}

		size_t middleIndex = aVector.size() / 2;
		std::vector<T> leftVector(aVector.begin(), aVector.begin() + middleIndex);
		std::vector<T> rightVector(aVector.begin() + middleIndex, aVector.end());

		MergeSort(leftVector);
		MergeSort(rightVector);

		aVector.clear();

		size_t leftIndex = 0;
		size_t rightIndex = 0;

		while (leftIndex < leftVector.size() && rightIndex < rightVector.size())
		{
			if (leftVector[leftIndex] < rightVector[rightIndex])
			{
				aVector.push_back(leftVector[leftIndex]);
				leftIndex++;
			}
			else
			{
				aVector.push_back(rightVector[rightIndex]);
				rightIndex++;
			}
		}

		while (leftIndex < leftVector.size())
		{
			aVector.push_back(leftVector[leftIndex]);
			leftIndex++;
		}

		while (rightIndex < rightVector.size())
		{
			aVector.push_back(rightVector[rightIndex]);
			rightIndex++;
		}
	};
}