#pragma once

struct BaseCommand
{
	virtual ~BaseCommand() {};
	virtual bool Execute() = 0;
	virtual bool Undo() = 0;
};

template<typename T>
struct ValueCommand : public BaseCommand
{
	ValueCommand(T* ptr, const T& from, const T& to) : myValuePtr(ptr), myFrom(from), myTo(to), BaseCommand() { }

	virtual bool Execute() override;
	virtual bool Undo() override;

private:
	T* myValuePtr;
	const T myFrom;
	const T myTo;
};

template<typename T>
bool ValueCommand<T>::Execute()
{
	*myValuePtr = myTo;
	return true;
}

template<typename T>
bool ValueCommand<T>::Undo()
{
	*myValuePtr = myFrom;
	return true;
}
