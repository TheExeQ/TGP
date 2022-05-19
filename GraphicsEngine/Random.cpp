#include "Random.h"

std::random_device Random::myRd;

int Random::GetRandomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(myRd);
}

float Random::GetRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(myRd);
}

bool Random::GetRandomBool()
{
	std::uniform_int_distribution<int> dist(0, 1);
	return dist(myRd);
}
