#pragma once
<<<<<<< HEAD

template <class T>
class Stack
{
public:
	Stack();

	int GetSize() const;

	const T& GetTop() const;
	T& GetTop();

	void Push(const T& aValue);
	T Pop();

private:
	std::vector<T> myStack;
};

template <class T>
T Stack<T>::Pop()
{

}

template <class T>
void Stack<T>::Push(const T& aValue)
{

}

template <class T>
T& Stack<T>::GetTop()
{

}

template <class T>
const T& Stack<T>::GetTop() const
{

}

template <class T>
int Stack<T>::GetSize() const
{

}

template <class T>
Stack<T>::Stack()
{

=======
#include <vector>
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Stack
	{
	public:
		Stack();

		int GetSize() const;

		const T& GetTop() const;
		T& GetTop();

		void Push(const T& aValue);
		T Pop();

	private:
		std::vector<T> myStack;
	};

	template <class T>
	T Stack<T>::Pop()
	{
		assert(myStack.size() > 0 && "Stack is empty");
		if (myStack.size() <= 0) return T();
		T temp = myStack.back();
		myStack.pop_back();
		return temp;
	}

	template <class T>
	void Stack<T>::Push(const T& aValue)
	{
		myStack.emplace_back(aValue);
	}

	template <class T>
	T& Stack<T>::GetTop()
	{
		assert(myStack.size() > 0 && "Stack is empty");
		return myStack.back();
	}

	template <class T>
	const T& Stack<T>::GetTop() const
	{
		assert(myStack.size() > 0 && "Stack is empty");
		return myStack.back();
	}

	template <class T>
	int Stack<T>::GetSize() const
	{
		return myStack.size();
	}

	template <class T>
	Stack<T>::Stack()
	{

	}
>>>>>>> 90442be70fda61db8c7d970b5873eb3c862127b3
}
