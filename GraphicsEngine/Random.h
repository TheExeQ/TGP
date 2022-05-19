#pragma once
#include <random>

class Random
{
	public:
	Random() = default;
	~Random() = default;

	static int GetRandomInt(int min, int max);
	static float GetRandomFloat(float min, float max);
	static bool GetRandomBool();
	
private:
	static std::random_device myRd;
};

