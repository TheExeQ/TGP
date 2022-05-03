#include "GraphicsEngine.pch.h"
#include "../CU/InputHandler.hpp"
#include <iostream>

CommonUtilities::InputHandler::InputHandler()
	:
	myCurrentMousePos{ 0, 0 },
	myPrevMousePos{ 0, 0 }
{
	if (myInstance == nullptr)
	{
		myInstance = this;
	}
}

CommonUtilities::InputHandler* CommonUtilities::InputHandler::GetInstance()
{
	return myInstance;
}

POINT CommonUtilities::InputHandler::GetMousePosition()
{
	return myCurrentMousePos;
}

POINT CommonUtilities::InputHandler::GetMouseMovement()
{
	POINT p;

	p.x = myCurrentMousePos.x - myPrevMousePos.x;
	p.y = myCurrentMousePos.y - myPrevMousePos.y;

	return p;
}

CommonUtilities::InputHandler::Event CommonUtilities::InputHandler::ReadKey()
{
	// TODO: Change to return optional
	CommonUtilities::InputHandler::Event nullEvent(Event::Type::Invalid, '0');
	if (myKeybuffer.size() > 0u)
	{
		CommonUtilities::InputHandler::Event e = myKeybuffer.front();
		myKeybuffer.pop();
		return e;
	}
	return nullEvent;
}

CommonUtilities::InputHandler::Event CommonUtilities::InputHandler::ReadChar()
{
	// TODO: Change to return optional
	CommonUtilities::InputHandler::Event nullEvent(Event::Type::Invalid, '0');
	if (myCharbuffer.size() > 0u)
	{
		CommonUtilities::InputHandler::Event e = myCharbuffer.front();
		myCharbuffer.pop();
		return e;
	}
	return nullEvent;
}

bool CommonUtilities::InputHandler::IsKeyPressed(unsigned char aKeyCode)
{
	if (myKeystates[aKeyCode])
	{
		myKeystates[aKeyCode] = false;
		return true;
	}
	else
	{
		return myKeystates[aKeyCode];
	}
}

bool CommonUtilities::InputHandler::IsKeyDown(unsigned char aKeyCode) const
{
	return myKeystates[aKeyCode] || myKeystatesHold[aKeyCode];
}

bool CommonUtilities::InputHandler::KeyIsEmpty() const
{
	return myKeybuffer.empty();
}

bool CommonUtilities::InputHandler::CharIsEmpty() const
{
	return myCharbuffer.empty();
}

void CommonUtilities::InputHandler::FlushKey()
{
	myKeybuffer = std::queue<Event>();
}

void CommonUtilities::InputHandler::FlushChar()
{
	myCharbuffer = std::queue<Event>();
}

void CommonUtilities::InputHandler::Flush()
{
	FlushKey();
	FlushChar();
}

void CommonUtilities::InputHandler::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!(lParam & (1 << 30)))
		{
			OnKeyPressed(wParam);
		}
		else
		{
			myKeystates[wParam] = false;
			OnKeyHold(wParam);
		}
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		OnKeyReleased(wParam);
		break;

	case WM_CHAR:
		OnChar(wParam);
		break;

	case WM_MOUSEMOVE:
		myPrevMousePos = myCurrentMousePos;
		GetCursorPos(&myCurrentMousePos);
		break;

	case WM_LBUTTONDOWN:
		OnKeyPressed(0x01);
		break;

	case WM_LBUTTONUP:
		OnKeyReleased(0x01);
		break;

	case WM_RBUTTONDOWN:
		OnKeyPressed(0x02);
		break;

	case WM_RBUTTONUP:
		OnKeyReleased(0x02);
		break;

	case WM_MBUTTONDOWN:
		OnKeyPressed(0x04);
		break;

	case WM_MBUTTONUP:
		OnKeyReleased(0x04);
		break;

	case WM_KILLFOCUS:
		ClearState();
		break;

	default:
		break;
	}
}

void CommonUtilities::InputHandler::OnKeyPressed(unsigned char aKeyCode)
{
	myKeystates[aKeyCode] = true;
	myKeybuffer.push(CommonUtilities::InputHandler::Event(CommonUtilities::InputHandler::Event::Type::Press, aKeyCode));
	TrimBuffer(myKeybuffer);
}

void CommonUtilities::InputHandler::OnKeyHold(unsigned char aKeyCode)
{
	myKeystatesHold[aKeyCode] = true;
}

void CommonUtilities::InputHandler::OnKeyReleased(unsigned char aKeyCode)
{
	myKeystates[aKeyCode] = false;
	myKeystatesHold[aKeyCode] = false;
	myKeybuffer.push(CommonUtilities::InputHandler::Event(CommonUtilities::InputHandler::Event::Type::Release, aKeyCode));
	TrimBuffer(myKeybuffer);
}

void CommonUtilities::InputHandler::OnChar(char aChar)
{
	myCharbuffer.push(CommonUtilities::InputHandler::Event(CommonUtilities::InputHandler::Event::Type::Press, aChar));
	TrimBuffer(myCharbuffer);
}

void CommonUtilities::InputHandler::ClearState()
{
	myKeystates.reset();
	myKeystatesHold.reset();
}

template<typename T>
void CommonUtilities::InputHandler::TrimBuffer(std::queue<T>& aBuffer)
{
	while (aBuffer.size() > myBufferSize)
	{
		aBuffer.pop();
	}
}