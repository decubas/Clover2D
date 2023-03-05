#include "random.h"



	static std::random_device rd;
	static std::mt19937 mt;

	void Random::Init()
	{
		mt = std::mt19937(rd());
	}

	int Random::irand(int a, int b)
	{
		std::uniform_int_distribution<int> intDist(a, b);
		return intDist(mt);
	}

	float Random::frand(float a, float b)
	{
		std::uniform_real_distribution<float> floatDist(a, b);
		return floatDist(mt);;
	}

