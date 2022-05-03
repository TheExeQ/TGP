#pragma once
#include <array>
#include <Windows.h>
#include <queue>
#include <bitset>

// Win32/DX11 InputHandler System

namespace CommonUtilities
{
	class InputHandler
	{
	public:
		class Event
		{
		public:
			enum class Type
			{
				Press,
				Release,
				Invalid
			};
		private:
			Type myType;
			unsigned char myCode;
		public:
			Event(Type aType, unsigned char aCode)
				:
				myType(aType),
				myCode(aCode)
			{}
			bool IsPress() const
			{
				return myType == Type::Press;
			}
			bool IsRelease() const
			{
				return myType == Type::Release;
			}
			unsigned char GetCode() const
			{
				return myCode;
			}
		};
		
		InputHandler();
		static InputHandler* GetInstance();

		// Mouse events
		POINT GetMousePosition();
		POINT GetMouseMovement();

		// Key events
		bool IsKeyPressed(unsigned char aKeyCode);
		bool IsKeyDown(unsigned char aKeyCode) const;
		Event ReadKey();
		bool KeyIsEmpty() const;
		void FlushKey();

		// Char events
		Event ReadChar();
		bool CharIsEmpty() const;
		void FlushChar();
		void Flush();

		void UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void OnKeyPressed(unsigned char aKeyCode);
		void OnKeyHold(unsigned char aKeyCode);
		void OnKeyReleased(unsigned char aKeyCode);
		void OnChar(char aChar);
		void ClearState();
		template<typename T>
		static void TrimBuffer(std::queue<T>& aBuffer);

	private:
		inline static InputHandler* myInstance;

		static constexpr unsigned int myNKeys = 256u;
		static constexpr unsigned int myBufferSize = 16u;
		std::bitset<myNKeys> myKeystates;
		std::bitset<myNKeys> myKeystatesHold;
		std::queue<Event> myKeybuffer;
		std::queue<Event> myCharbuffer;

		POINT myCurrentMousePos;
		POINT myPrevMousePos;
	};

	enum KeyCode : int
	{
		MOUSELBUTTON = 0x01,
		MOUSERBUTTON = 0x02,
		CANCEL = 0x03,
		MBUTTON = 0x04,

		MOUSEXBUTTON1 = 0x05,
		MOUSEXBUTTON2 = 0x06,

		BACK = 0x08,
		TAB = 0x09,

		CLEAR = 0x0C,
		RETURN = 0x0D,

		SHIFT = 0x10,
		CONTROL = 0x11,
		MENU = 0x12,
		PAUSE = 0x13,
		CAPITAL = 0x14,

		ESCAPE = 0x1B,

		SPACE = 0x20,
		PRIOR = 0x21, //page up
		NEXT = 0x22, //page down
		END = 0x23,
		HOME = 0x24,
		LEFT = 0x25,
		UP = 0x26,
		RIGHT = 0x27,
		DOWN = 0x28,
		SELECT = 0x29,
		PRINT = 0x2A,
		EXECUTE = 0x2B,
		SNAPSHOT = 0x2C,
		INSERT = 0x2D,
		DELETE_BUTTON = 0x2E,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,


		NUMPAD0 = 0x60,
		NUMPAD1 = 0x61,
		NUMPAD2 = 0x62,
		NUMPAD3 = 0x63,
		NUMPAD4 = 0x64,
		NUMPAD5 = 0x65,
		NUMPAD6 = 0x66,
		NUMPAD7 = 0x67,
		NUMPAD8 = 0x68,
		NUMPAD9 = 0x69,
		MULTIPLY = 0x6A,
		ADD = 0x6B,
		SEPARATOR = 0x6C,
		SUBTRACT = 0x6D,
		DIVIDE = 0x6F,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,
		SCROLL = 0x91,

		LSHIFT = 0xA0,
		RSHIFT = 0xA1,
		LCONTROL = 0xA2,
		RCONTROL = 0xA3,
		LMENU = 0xA4,
		RMENU = 0xA5,

		OEM_PLUS = 0xBB,   // '+' any country
		OEM_COMMA = 0xBC,   // ',' any country
		OEM_MINUS = 0xBD,   // '-' any country
		OEM_PERIOD = 0xBE,   // '.' any country

		GAMEPAD_A = 0xC3, // reserved
		GAMEPAD_B = 0xC4, // reserved
		GAMEPAD_X = 0xC5, // reserved
		GAMEPAD_Y = 0xC6, // reserved
		GAMEPAD_RIGHT_SHOULDER = 0xC7, // reserved
		GAMEPAD_LEFT_SHOULDER = 0xC8, // reserved
		GAMEPAD_LEFT_TRIGGER = 0xC9, // reserved
		GAMEPAD_RIGHT_TRIGGER = 0xCA, // reserved
		GAMEPAD_DPAD_UP = 0xCB, // reserved
		GAMEPAD_DPAD_DOWN = 0xCC, // reserved
		GAMEPAD_DPAD_LEFT = 0xCD, // reserved
		GAMEPAD_DPAD_RIGHT = 0xCE, // reserved
		GAMEPAD_MENU = 0xCF, // reserved
		GAMEPAD_VIEW = 0xD0, // reserved
		GAMEPAD_LEFT_THUMBSTICK_BUTTON = 0xD1, // reserved
		GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2, // reserved
		GAMEPAD_LEFT_THUMBSTICK_UP = 0xD3, // reserved
		GAMEPAD_LEFT_THUMBSTICK_DOWN = 0xD4, // reserved
		GAMEPAD_LEFT_THUMBSTICK_RIGHT = 0xD5, // reserved
		GAMEPAD_LEFT_THUMBSTICK_LEFT = 0xD6, // reserved
		GAMEPAD_RIGHT_THUMBSTICK_UP = 0xD7, // reserved
		GAMEPAD_RIGHT_THUMBSTICK_DOWN = 0xD8, // reserved
		GAMEPAD_RIGHT_THUMBSTICK_RIGHT = 0xD9, // reserved
		GAMEPAD_RIGHT_THUMBSTICK_LEFT = 0xDA, // reserved
	};
}


