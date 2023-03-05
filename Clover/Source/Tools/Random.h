#ifndef _RANDOM__H
#define _RANDOM__H

#include <random>


	class Random
	{
	public:
		static void Init();
		static int irand(int a, int b);
		static float frand(float a, float b);
	};

#endif
