#ifndef CLOCK_H_
#define CLOCK_H_ 1

#include <inttypes.h>
#include <chrono>
#include "Delegate/DelegateBase.h"


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

	class UniqueID;
	
	Delegate(OnTimerTick)
	struct TimerHandler
	{
		OnTimerTick Tick;
		float fTime = 1.f;
		bool bLoop = false;

		//Determinate if this timer should be removed after the update
		bool bFinished = false;

		UniqueID Id;
		float fAccumTime = 0.f;
	};

	class TimerManager
	{
	public:
		void UpdateTimers(float fDeltaTime);

		void AddTimer(TimerHandler& handle);
		template<class T>
		TimerHandler& AddTimer(void* owner, void(T::* function_)(void), float time, bool loop = false)
		{
			TimerHandler handle;
			handle.Tick.AddFunction((T*)owner, function_);
			handle.fTime = time;
			handle.bLoop = loop;
			Timers[handle.Id] = handle;
			return Timers[handle.Id];
		}

		void RemoveTimer(const TimerHandler& handle);

	private:
		std::unordered_map<UniqueID, TimerHandler> Timers;
	};








#endif