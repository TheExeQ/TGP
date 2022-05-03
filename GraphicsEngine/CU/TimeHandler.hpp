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
		void Update();
		float GetDeltaTime() const;
		double GetTotalTime() const;

	private:
		std::chrono::time_point<std::chrono::steady_clock> myStart, myLastFrame;
		std::chrono::duration<double> myDeltaTime, myTotalTime;
	};
}

