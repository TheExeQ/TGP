#pragma once
#include "Commands.h"

#include <stack>

class CommandManager
{
public:
	static void DoCommand(BaseCommand* command)
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
		myUndoStack.top()->Undo();
		myRedoStack.push(myUndoStack.top());
		myUndoStack.pop();
	};

	static void Redo()
	{
		myRedoStack.top()->Execute();
		myUndoStack.push(myRedoStack.top());
		myRedoStack.pop();
	};

private:
	static std::stack<BaseCommand*> myUndoStack;
	static std::stack<BaseCommand*> myRedoStack;
};