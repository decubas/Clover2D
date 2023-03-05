#include "Clover.h"
#include <Tools/Timer.h>
#include "UniqueId.h"

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

	void TimerManager::UpdateTimers(float fDeltaTime)
	{
		bool bDirty = false;
		for (auto& pair : Timers)
		{
			TimerHandler& handle = pair.second;
			if (handle.fAccumTime >= handle.fTime)
			{
				handle.Tick.Broadcast();
				handle.fAccumTime = 0.f;
				if (false == handle.bLoop)
				{
					bDirty = true;
					handle.bFinished = true;
				}
			}
			handle.fAccumTime += fDeltaTime;
		}

		// Remove finished timers
		if (bDirty)
		{
			std::unordered_map<UniqueID, TimerHandler> temp = Timers;
			for (auto& pair : temp)
			{
				TimerHandler& handle = pair.second;
				if (handle.bFinished)
				{
					RemoveTimer(handle);
				}
			}
		}
	}


	void TimerManager::AddTimer(TimerHandler& handle)
	{
		Timers[handle.Id] = handle;
	}

	void TimerManager::RemoveTimer(const TimerHandler& handle)
	{
		std::unordered_map<UniqueID, TimerHandler>::iterator position = Timers.find(handle.Id);
		if (position == Timers.end())
		{
			return;
		}
		Timers.erase(handle.Id);
	}
