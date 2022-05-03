#pragma once
#include <memory>

namespace CommonUtilities
{
	template <class T>
	class BSTSet;

	template <class T>
	class BSTNode : public std::enable_shared_from_this<BSTNode<T>>
	{
		friend BSTSet<T>;
		void SetValue(const T& aValue);
		std::shared_ptr<BSTNode<T>> GetValue(const T& aValue);

		T myValue;
		std::shared_ptr<BSTNode<T>> myMinValue;
		std::shared_ptr<BSTNode<T>> myMaxValue;
	};

	template <class T>
	std::shared_ptr<BSTNode<T>> BSTNode<T>::GetValue(const T& aValue)
	{
		if (myValue == aValue) 
			return this->shared_from_this();
		else
		{
			if (aValue < myValue)
			{
				if (!myMinValue) return std::make_shared<BSTNode<T>>();
				return myMinValue->GetValue(aValue);
			}
			else
			{
				if (!myMaxValue) return std::make_shared<BSTNode<T>>();
				return myMaxValue->GetValue(aValue);
			}
		}
	}

	template <class T>
	void BSTNode<T>::SetValue(const T& aValue)
	{
		if (myValue == aValue) { return; }
		if (myValue > aValue)
		{
			if (!myMinValue)
			{
				myMinValue = std::make_shared<BSTNode<T>>();
				myMinValue->myValue = aValue;
			}
			else
			{
				myMinValue->SetValue(aValue);
			}
		}
		else
		{
			if (!myMaxValue)
			{
				myMaxValue = std::make_shared<BSTNode<T>>();
				myMaxValue->myValue = aValue;
			}
			else
			{
				myMaxValue->SetValue(aValue);
			}
		}
	}

	template <class T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();

		bool HasElement(const T& aValue);
		void Insert(const T& aValue);
		void Remove(const T& aValue);

	private:
		void deleteByMerging(std::shared_ptr<BSTNode<T>>& aNode);
		void findAndDeleteByMerging(const T& aValue);

		std::shared_ptr<BSTNode<T>> myRootNode;
	};

	template <class T>
	void BSTSet<T>::Remove(const T& aValue)
	{
		findAndDeleteByMerging(aValue);
		if (!myRootNode) 
		{
			myRootNode = std::make_shared<BSTNode<T>>();
			myRootNode->myValue = 0;
		}
	}

	template <class T>
	void BSTSet<T>::Insert(const T& aValue)
	{
		if (myRootNode->myValue == 0)
		{
			myRootNode->myValue = aValue;
		}
		else
		{
			myRootNode->SetValue(aValue);
		}
	}

	template <class T>
	bool BSTSet<T>::HasElement(const T& aValue)
	{
		if (!myRootNode) return false;
		else if (myRootNode->GetValue(aValue)->myValue == aValue) return true;
		else false;
	}

	template <class T>
	BSTSet<T>::~BSTSet()
	{

	}

	template <class T>
	BSTSet<T>::BSTSet()
	{
		myRootNode = std::make_shared<BSTNode<T>>();
		myRootNode->myValue = 0;
	}

	template<class T>
	void BSTSet<T>::deleteByMerging(std::shared_ptr<BSTNode<T>>& aNode)
	{
		std::shared_ptr<BSTNode<T>> tmp = aNode;
		if (aNode) 
		{
			if (!aNode->myMaxValue)
			{
				aNode = aNode->myMinValue;
			}
			else if (!aNode->myMinValue)
			{
				aNode = aNode->myMaxValue;
			}
			else 
			{ 
				tmp = aNode->myMinValue;
				while (tmp->myMaxValue != 0)
				{
					tmp = tmp->myMaxValue;
				}
				tmp->myMaxValue = aNode->myMaxValue;
				aNode = aNode->myMinValue;
			}
		}
	}

	template<class T>
	void BSTSet<T>::findAndDeleteByMerging(const T& aValue) 
	{
		std::shared_ptr<BSTNode<T>> node = myRootNode;
		std::shared_ptr<BSTNode<T>> prev;

		while (node) 
		{
			if (node->myValue == aValue)
			{
				break;
			}
			prev = node;
			if (aValue < node->myValue)
			{
				node = node->myMinValue;
			}
			else
			{
				node = node->myMaxValue;
			}
		}
		if (node && node->myValue == aValue)
		{
			if (node == myRootNode)
			{
				deleteByMerging(myRootNode);
			}
			else if (prev->myMinValue == node)
			{
				deleteByMerging(prev->myMinValue);
			}
			else
			{
				deleteByMerging(prev->myMaxValue);
			}
		}
	}
}
