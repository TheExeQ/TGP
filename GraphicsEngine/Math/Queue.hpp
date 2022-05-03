#pragma once
#include <vector>
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		Queue();

		int GetSize() const;

		const T& GetFront() const;
		T& GetFront();

		void Enqueue(const T& aValue);
		T Dequeue();
	private:
		std::vector<T> myQueue;
		int myFirst;
		int myLast;
		int mySize;
	};

	template <class T>
	T Queue<T>::Dequeue()
	{
		assert(mySize > 0 && "Queue size is empty");
		myFirst++;
		mySize--;
		if (!myQueue.size()) return 0;
		else if (mySize == 0)
		{
			auto returnValue = myQueue[myFirst - 1];
			myFirst = 0;
			myLast = 0;
			mySize = 0;
			myQueue.clear();
			return returnValue;
		}
		else if (myFirst >= myQueue.size()) 
		{
			myFirst = 0;
			return myQueue[myQueue.size() - 1];
		}
		return myQueue[myFirst - 1];
	}

	template <class T>
	void Queue<T>::Enqueue(const T& aValue)
	{
		if (myFirst == 0)
		{
			myQueue.emplace_back(aValue);
			myLast++;
		}
		else if (myFirst == myLast && mySize > 0)
		{
			auto it = myQueue.begin() + myLast;
			myQueue.insert(it, aValue);
			myLast++;
			myFirst++;
		}
		else if (myFirst > 0)
		{
			if (myFirst < myLast) myLast = 1;
			else myLast++;
			myQueue[myLast - 1] = aValue;
		}
		mySize++;
	}

	template <class T>
	T& Queue<T>::GetFront()
	{
		assert(mySize > 0 && "Queue size is empty");
		return myQueue[myFirst];
	}

	template <class T>
	const T& Queue<T>::GetFront() const
	{
		assert(mySize > 0 && "Queue size is empty");
		return myQueue[myFirst];
	}

	template <class T>
	int Queue<T>::GetSize() const
	{
		return mySize;
	}

	template <class T>
	Queue<T>::Queue()
	{
		myFirst = 0;
		myLast = 0;
		mySize = 0;
	}
}
