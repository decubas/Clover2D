#include "Clover.h"
#include <Tools/Timer.h>

	Clock::Clock()
	{
		reset();
	}

	Clock::~Clock()
	{
	}

	void Clock::reset()
	{
		m_tp1 = std::chrono::system_clock::now();
		m_tp2 = std::chrono::system_clock::now();
	}

	float Clock::get()
	{
		// Handle Timing
		m_tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
		m_tp1 = m_tp2;

		// Our time per frame coefficient
		float fElapsedTime = elapsedTime.count();
		fLastElapsed = fElapsedTime;
		return fElapsedTime;
	}
