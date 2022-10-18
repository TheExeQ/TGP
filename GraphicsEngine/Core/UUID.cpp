#include "gepch.h"
#include "UUID.h"

#include <random>
#include <unordered_map>


static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

TGA::UUID::UUID()
	: myUUID(s_UniformDistribution(s_Engine))
{
}

TGA::UUID::UUID(uint64_t uuid)
	: myUUID(uuid)
{
}
