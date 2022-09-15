#pragma once
#include "Commands.h"
#include "Core/Base.h"

#include <stack>

class CommandManager
{
public:
	static void DoCommand(Ref<BaseCommand> command)
	{
		command->Execute();
		while (!myRedoStack.empty())
		{
			myRedoStack.pop();
		}
		myUndoStack.push(command);
	};

	static void Undo()
	{
		if (!myUndoStack.empty())
		{
			myUndoStack.top()->Undo();
			myRedoStack.push(myUndoStack.top());
			myUndoStack.pop();
		}
	};

	static void Redo()
	{
		if (!myRedoStack.empty())
		{
			myRedoStack.top()->Execute();
			myUndoStack.push(myRedoStack.top());
			myRedoStack.pop();
		}
	};

private:
	inline static std::stack<Ref<BaseCommand>> myUndoStack;
	inline static std::stack<Ref<BaseCommand>> myRedoStack;
};