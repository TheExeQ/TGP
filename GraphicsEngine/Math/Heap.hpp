#pragma once
#include <assert.h>
#include <vector>

namespace CommonUtilities
{
	template <class T>
	class Heap
	{
	public:
		int GetSize() const;
		void Enqueue(const T& aElement);
		const T& GetTop() const;
		T Dequeue();

	private:
		void SortChild(T& outChild, T& outParent);

		std::vector<T> myElements;
	};

	template <class T>
	void CommonUtilities::Heap<T>::SortChild(T& outChild, T& outParent)
	{
		if (outParent < outChild)
		{
			T temp = outParent;
			outParent = outChild;
			outChild = temp;
		}
	}

	template <class T>
	T Heap<T>::Dequeue()
	{
		short index = 0;
		short leftChildIndex = (2 * index + 1);
		short rightChildIndex = (2 * index + 2);

		auto returnValue = myElements[index];
		myElements[index] = myElements.back();
		myElements.pop_back();

		bool hasLeftChild = (leftChildIndex < GetSize());
		bool hasRightChild = (rightChildIndex < GetSize());

		bool isSortedCorrectly = false;

		while (!isSortedCorrectly)
		{
			if (hasLeftChild && hasRightChild)
			{
				if (myElements[rightChildIndex] < myElements[leftChildIndex])
				{
					SortChild(myElements[leftChildIndex], myElements[index]);
					index = leftChildIndex;
				}
				else
				{
					SortChild(myElements[rightChildIndex], myElements[index]);
					index = rightChildIndex;
				}
			}
			else if (hasLeftChild)
			{
				SortChild(myElements[leftChildIndex], myElements[index]);
					index = leftChildIndex;
			}
			else if (hasRightChild)
			{
				SortChild(myElements[rightChildIndex], myElements[index]);
				index = rightChildIndex;
			}

			leftChildIndex = (2 * index + 1);
			rightChildIndex = (2 * index + 2);

			hasLeftChild = (leftChildIndex < GetSize());
			hasRightChild = (rightChildIndex < GetSize());

			isSortedCorrectly = (!hasLeftChild && !hasRightChild) ? true : false;
		}

		return returnValue;
	}

	template <class T>
	const T& Heap<T>::GetTop() const
	{
		assert(GetSize() > 0, "Heap is empty.");
		return myElements[0];
	}

	template <class T>
	void Heap<T>::Enqueue(const T& aElement)
	{
		myElements.emplace_back(aElement);

		auto index = GetSize() - 1;
		auto parentIndex = static_cast<int>((index - 1) * 0.5f);

		bool isSortedCorrectly = (myElements[index] < myElements[parentIndex] || !(myElements[index] < myElements[parentIndex]) && !(myElements[parentIndex] < myElements[index])) ? true : false;
		while (!isSortedCorrectly)
		{
			T temp = myElements[index];
			myElements[index] = myElements[parentIndex];
			myElements[parentIndex] = temp;

			index = parentIndex;
			parentIndex = static_cast<int>((index - 1) * 0.5f);
			isSortedCorrectly = (myElements[index] < myElements[parentIndex] || !(myElements[index] < myElements[parentIndex]) && !(myElements[parentIndex] < myElements[index])) ? true : false;
		}
	}

	template <class T>
	int Heap<T>::GetSize() const
	{
		return myElements.size();
	}
}