#pragma once

namespace CommonUtilities
{
	template <class T>
	class DoubleLinkedList;

	template <class T>
	class DoubleLinkedListNode
	{
	public:
		DoubleLinkedListNode<T>(const DoubleLinkedListNode<T>&) = delete;
		DoubleLinkedListNode<T>& operator=(const DoubleLinkedListNode<T>&) = delete;

		const T& GetValue() const;
		T& GetValue();
		DoubleLinkedListNode<T>* GetNext() const;
		DoubleLinkedListNode<T>* GetPrevious() const;

	private:
		friend class DoubleLinkedList<T>;
		DoubleLinkedListNode(const T& aValue);
		~DoubleLinkedListNode() {}

		T value;
		DoubleLinkedListNode<T>* prev;
		DoubleLinkedListNode<T>* next;
	};

	template <class T>
	class DoubleLinkedList
	{
	public:
		DoubleLinkedList();
		~DoubleLinkedList();

		int GetSize() const;
		DoubleLinkedListNode<T>* GetFirst();
		DoubleLinkedListNode<T>* GetLast();
		void InsertFirst(const T& aValue);
		void InsertLast(const T& aValue);
		void InsertBefore(DoubleLinkedListNode<T>* aNode, const T& aValue);
		void InsertAfter(DoubleLinkedListNode<T>* aNode, const T& aValue);
		void Remove(DoubleLinkedListNode<T>* aNode);
		DoubleLinkedListNode<T>* FindFirst(const T& aValue);
		DoubleLinkedListNode<T>* FindLast(const T& aValue);
		bool RemoveFirst(const T& aValue);
		bool RemoveLast(const T& aValue);

	private:
		DoubleLinkedListNode<T>* myHead;
		DoubleLinkedListNode<T>* myTail;
		int myCount;
	};

	template <class T>
	CommonUtilities::DoubleLinkedListNode<T>::DoubleLinkedListNode(const T& aValue)
	{
		value = aValue;
	}

	template <class T>
	CommonUtilities::DoubleLinkedListNode<T>* CommonUtilities::DoubleLinkedListNode<T>::GetPrevious() const
	{
		return prev;
	}

	template <class T>
	CommonUtilities::DoubleLinkedListNode<T>* CommonUtilities::DoubleLinkedListNode<T>::GetNext() const
	{
		return next;
	}

	template <class T>
	T& CommonUtilities::DoubleLinkedListNode<T>::GetValue()
	{
		return value;
	}

	template <class T>
	const T& CommonUtilities::DoubleLinkedListNode<T>::GetValue() const
	{
		return value;
	}

	template <class T>
	bool CommonUtilities::DoubleLinkedList<T>::RemoveLast(const T& aValue)
	{
		auto* node = myTail;
		if (!node) { return false; }
		do
		{
			if (node->value == aValue)
			{
				if (node != myHead && node != myTail)
				{
					node->prev->next = node->next;
					node->next->prev = node->prev;
					delete node;
					myCount--;
					return true;
				}
				else if (node == myHead)
				{
					if (node->next) { node->next->prev = nullptr; }
					if (myTail == myHead) { myTail = nullptr; }
					myHead = node->next;
					delete node;
					myCount--;
					return true;
				}
				else if (node == myTail)
				{
					if (node->prev) { node->prev->next = nullptr; }
					if (myHead == myTail) { myHead = nullptr; }
					myTail = node->prev;
					delete node;
					myCount--;
					return true;
				}
			}
			node = node->prev;
		} while (node);
		return false;
	}

	template <class T>
	bool CommonUtilities::DoubleLinkedList<T>::RemoveFirst(const T& aValue)
	{
		auto* node = myHead;
		if (!node) { return false; }
		do
		{
			if (node->value == aValue)
			{
				if (node != myHead && node != myTail)
				{
					node->prev->next = node->next;
					node->next->prev = node->prev;
					delete node;
					myCount--;
					return true;
				}
				else if (node == myHead)
				{
					if (node->next) { node->next->prev = nullptr; }
					if (myTail == myHead) { myTail = nullptr; }
					myHead = node->next;
					delete node;
					myCount--;
					return true;
				}
				else if (node == myTail)
				{
					if (node->prev) { node->prev->next = nullptr; }
					if (myHead == myTail) { myHead = nullptr; }
					myTail = node->prev;
					delete node;
					myCount--;
					return true;
				}
			}
			node = node->next;
		} while (node);
		return false;
	}

	template <class T>
	CommonUtilities::DoubleLinkedListNode<T>* CommonUtilities::DoubleLinkedList<T>::FindLast(const T& aValue)
	{
		auto* node = myTail;
		do
		{
			if (node->value == aValue)
			{
				return node;
			}
			node = node->prev;
		} while (node);
		return nullptr;
	}

	template <class T>
	CommonUtilities::DoubleLinkedListNode<T>* CommonUtilities::DoubleLinkedList<T>::FindFirst(const T& aValue)
	{
		auto* node = myHead;
		do
		{
			if (node->value == aValue)
			{
				return node;
			}
			node = node->next;
		} while (node);
		return nullptr;
	}

	template <class T>
	void CommonUtilities::DoubleLinkedList<T>::Remove(DoubleLinkedListNode<T>* aNode)
	{
		if (!aNode) { return; }
		if (aNode != myHead && aNode != myTail)
		{
			aNode->prev->next = aNode->next;
			aNode->next->prev = aNode->prev;
			delete aNode;
			myCount--;
		}
		else if (aNode == myHead)
		{
			if (aNode->next) { aNode->next->prev = nullptr; }
			myHead = aNode->next;
			delete aNode;
			myCount--;
		}
		else if (aNode == myTail)
		{
			if (aNode->prev) { aNode->prev->next = nullptr; }
			myTail = aNode->prev;
			delete aNode;
			myCount--;
		}
	}

	template <class T>
	void CommonUtilities::DoubleLinkedList<T>::InsertAfter(DoubleLinkedListNode<T>* aNode, const T& aValue)
	{
		auto* newNode = new DoubleLinkedListNode<T>(aValue);
		newNode->prev = aNode;
		newNode->next = aNode->next;
		if (newNode->next != nullptr) newNode->next->prev = newNode;
		else { myTail = newNode; }
		if (newNode->prev != nullptr) newNode->prev->next = newNode;
		myCount++;
	}

	template <class T>
	void CommonUtilities::DoubleLinkedList<T>::InsertBefore(DoubleLinkedListNode<T>* aNode, const T& aValue)
	{
		auto* newNode = new DoubleLinkedListNode<T>(aValue);
		newNode->prev = aNode->prev;
		newNode->next = aNode;
		if (newNode->next != nullptr) newNode->next->prev = newNode;
		if (newNode->prev != nullptr) newNode->prev->next = newNode;
		else { myHead = newNode; }
		myCount++;
	}

	template <class T>
	void CommonUtilities::DoubleLinkedList<T>::InsertLast(const T& aValue)
	{
		auto* newNode = new DoubleLinkedListNode<T>(aValue);
		newNode->prev = myTail;
		newNode->next = nullptr;
		myTail = newNode;
		if (!newNode->prev) { myHead = myTail; }
		else { myTail->prev->next = myTail; }
		myCount++;
	}

	template <class T>
	void CommonUtilities::DoubleLinkedList<T>::InsertFirst(const T& aValue)
	{
		auto* newNode = new DoubleLinkedListNode<T>(aValue);
		newNode->prev = nullptr;
		newNode->next = myHead;
		myHead = newNode;
		if (!newNode->next) { myTail = myHead; }
		else { myHead->next->prev = myHead; }
		myCount++;
	}

	template <class T>
	CommonUtilities::DoubleLinkedListNode<T>* CommonUtilities::DoubleLinkedList<T>::GetLast()
	{
		return myTail;
	}

	template <class T>
	CommonUtilities::DoubleLinkedListNode<T>* CommonUtilities::DoubleLinkedList<T>::GetFirst()
	{
		return myHead;
	}

	template <class T>
	int CommonUtilities::DoubleLinkedList<T>::GetSize() const
	{
		return myCount;
	}

	template <class T>
	CommonUtilities::DoubleLinkedList<T>::~DoubleLinkedList()
	{
		DoubleLinkedListNode<T>* currentNode;
		if (!myHead && !myTail) { return; }
		else if (myTail)
		{
			currentNode = myTail;
			while (currentNode->prev)
			{
				currentNode = currentNode->prev;
				if (!currentNode->next) { delete currentNode->next; }
			}
			delete myTail;
		}
		else
		{
			currentNode = myHead;
			while (currentNode->next)
			{
				currentNode = currentNode->next;
				if (!currentNode->prev) { delete currentNode->prev; }
			}
			delete myHead;
		}
	}

	template <class T>
	CommonUtilities::DoubleLinkedList<T>::DoubleLinkedList()
	{
		myHead = nullptr;
		myTail = nullptr;
		myCount = 0;
	}

}