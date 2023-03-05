#pragma once

#include <Clover.h>
#include "Sandbox/rectangle.h"
#include "Delegate/Delegate.h"
#include "Tools/Timer.h"

class Box2D;

class Minigame : public Box2D
{
public:
	Minigame();
	virtual ~Minigame();
	void OnActivate();
	void ResetCooldown();

	virtual void OnUpdate(float fDeltaTime) = 0;
	virtual void OnDraw() = 0;

protected:
	float m_MinCooldown = 1.f;
	float m_MaxCooldown = 2.f;
	bool m_bIsActive = false;
};
