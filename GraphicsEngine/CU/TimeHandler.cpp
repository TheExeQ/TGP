#include "GraphicsEngine.pch.h"
#include "../CU/TimeHandler.hpp"

std::chrono::time_point<std::chrono::steady_clock> CommonUtilities::Time::myStart, CommonUtilities::Time::myLastFrame;
std::chrono::duration<double> CommonUtilities::Time::myDeltaTime, CommonUtilities::Time::myTotalTime;

CommonUtilities::Time::Time()
{
	myStart = std::chrono::high_resolution_clock::now();
	myLastFrame = myStart;
	myDeltaTime = std::chrono::duration<double>(0);
	myTotalTime = std::chrono::duration<double>(0);
}

void CommonUtilities::Time::Update()
{
	myTotalTime = std::chrono::high_resolution_clock::now() - myStart;
	myDeltaTime = std::chrono::high_resolution_clock::now() - myLastFrame;
	myLastFrame = std::chrono::high_resolution_clock::now();
}

float CommonUtilities::Time::GetDeltaTime()
{
	return myDeltaTime.count();
}

double CommonUtilities::Time::GetTotalTime()
{
	return myTotalTime.count() * 1000;
}
