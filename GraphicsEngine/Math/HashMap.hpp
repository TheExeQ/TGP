#pragma once
#include <stdint.h>
#include <vector>
#include <string>

namespace CommonUtilities
{
	enum eHashState
	{
		Empty = 1 << 0,
		InUse = 1 << 1,
		Removed = 1 << 2,
	};

	template <class Key, class Value>
	class HashMap
	{
	public:
		HashMap(int aCapacity);
		~HashMap();
		bool Insert(const Key& aKey, const Value& aValue);
		bool Remove(const Key& aKey);
		const Value* Get(const Key& aKey) const;
		Value* Get(const Key& aKey);

	private:
		struct Entry
		{
			Key key;
			Value value;
			eHashState state;
		};

		const uint32_t mySize;
		Entry* myEntries;
	};

	template <class Key, class Value>
	CommonUtilities::HashMap<Key, Value>::~HashMap()
	{
		delete[] myEntries;
	}

	uint32_t Hash(const uint8_t* aBuffer, int count)
	{
		const uint32_t FNVOffsetBasis = 2166136261U;
		const uint32_t FNVPrime = 16777619U;
		uint32_t val = FNVOffsetBasis;
		for (int i = 0; i < count; ++i)
		{
			val ^= aBuffer[i];
			val *= FNVPrime;
		}
		return val;
	}

	template <class Key>
	uint32_t Hash(const Key& aKey)
	{
		return Hash(reinterpret_cast<const uint8_t*>(&aKey), sizeof(aKey));
	}

	uint32_t Hash(const std::string& aString)
	{
		return Hash(reinterpret_cast<const uint8_t*>(aString.c_str()), aString.size());
	}

	template <class Key, class Value>
	Value* HashMap<Key, Value>::Get(const Key& aKey)
	{
		bool loopedToFirst = false;
		uint32_t index = Hash(aKey) % mySize;
		uint32_t bindex = Hash(aKey) % mySize;

		while ((myEntries[index].key < aKey) || (aKey < myEntries[index].key))
		{
			index++;
			if (index >= mySize)
			{
				index = 0;
				loopedToFirst = true;
				if (loopedToFirst && index == Hash(aKey) % mySize) 
				{
					return nullptr;
				}
			}
			else if (loopedToFirst && index == Hash(aKey) % mySize)
			{
				return nullptr;
			}
		}
		if (myEntries[index].state == eHashState::Empty)
		{
			return nullptr;
		}
		return &myEntries[index].value;
	}

	template <class Key, class Value>
	const Value* HashMap<Key, Value>::Get(const Key& aKey) const
	{
		bool loopedToFirst = false;
		uint32_t index = Hash(aKey) % mySize;

		while ((myEntries[index].key < aKey) || (aKey < myEntries[index].key))
		{
			index++;
			if (index >= mySize)
			{
				index = 0;
				loopedToFirst = true;
				if (loopedToFirst && index == Hash(aKey) % mySize)
				{
					return nullptr;
				}
			}
			else if (loopedToFirst && index == Hash(aKey) % mySize)
			{
				return nullptr;
			}
		}
		if (myEntries[index].state == eHashState::Empty)
		{
			return nullptr;
		}
		return &myEntries[index].value;
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::Remove(const Key& aKey)
	{
		if (!myEntries || mySize <= 0)
		{
			return false;
		}

		bool loopedToFirst = false;
		uint32_t index = Hash(aKey) % mySize;

		while (myEntries[index].key < aKey || aKey < myEntries[index].key && myEntries[index].state != eHashState::Empty)
		{
			index++;
			if (index >= mySize)
			{
				index = 0;
				loopedToFirst = true;
				if (loopedToFirst && index == Hash(aKey) % mySize)
				{
					return nullptr;
				}
			}
			else if (loopedToFirst && index == Hash(aKey) % mySize)
			{
				return nullptr;
			}
		}
		myEntries[index].key = Key();
		myEntries[index].value = Value();
		myEntries[index].state = eHashState::Empty;
		return true;
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
	{
		if (!myEntries)
		{
			return false;
		}

		bool loopedToFirst = false;
		uint32_t index = Hash(aKey) % mySize;

		while (myEntries[index].state != eHashState::Empty && (myEntries[index].key < aKey || aKey < myEntries[index].key))
		{
			index++;
			if (index >= mySize)
			{
				index = 0;
				loopedToFirst = true;
				if (loopedToFirst && index == Hash(aKey) % mySize)
				{
					return nullptr;
				}
			}
			else if (loopedToFirst && index == Hash(aKey) % mySize)
			{
				return false;
			}
		}
		myEntries[index].key = aKey;
		myEntries[index].value = aValue;
		myEntries[index].state = eHashState::InUse;
		return true;
	}

	template <class Key, class Value>
	HashMap<Key, Value>::HashMap(int aCapacity) : mySize(aCapacity)
	{
		if (!mySize) 
		{
			myEntries = nullptr;
		}
		else
		{
			myEntries = new Entry[mySize];
		}
		for (uint32_t i = 0; i < mySize; i++)
		{
			myEntries[i].key = Key();
			myEntries[i].value = Value();
			myEntries[i].state = eHashState::Empty;
		}
	}
}
