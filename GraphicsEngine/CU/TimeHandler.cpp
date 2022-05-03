#include "GraphicsEngine.pch.h"
#include "../CU/TimeHandler.hpp"

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

float CommonUtilities::Time::GetDeltaTime() const
{
	return myDeltaTime.count();
}

double CommonUtilities::Time::GetTotalTime() const
{
	return myTotalTime.count() * 1000;
}
