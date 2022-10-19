#ifndef CLOCK_H_
#define CLOCK_H_ 1

#include <inttypes.h>
#include <chrono>


	//Singleton
	class Clock
	{
	public:

		Clock();
		~Clock();

		// Timer control
		void reset();
		float get();


	protected:
		float fLastElapsed;
		std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	};
#endif