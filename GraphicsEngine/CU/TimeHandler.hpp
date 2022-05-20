#pragma once
#include <chrono>

namespace CommonUtilities
{
	class Time
	{
	public:
		Time();
		Time(const Time& aTime) = delete;
		Time& operator=(const Time& aTime) = delete;
		static void Update();
		static float GetDeltaTime();
		static double GetTotalTime();

	private:
		static std::chrono::time_point<std::chrono::steady_clock> myStart, myLastFrame;
		static std::chrono::duration<double> myDeltaTime, myTotalTime;
	};
}

